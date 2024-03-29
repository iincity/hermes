#include "include/hermes_parser.h"
#include <string.h>


const char* DATA_TYPE_VOID = "void";
const char* DATA_TYPE_STRING = "string";
const char* DATA_TYPE_CHAR = "char";
const char* DATA_TYPE_INT = "int";
const char* DATA_TYPE_FLOAT = "float";
const char* DATA_TYPE_BOOLEAN = "bool";
const char* DATA_TYPE_OBJECT = "object";
const char* DATA_TYPE_REFERENCE = "ref";
const char* DATA_TYPE_LIST = "list";
const char* DATA_TYPE_SOURCE = "source";

const char* STATEMENT_WHILE = "while";
const char* STATEMENT_IF = "if";
const char* STATEMENT_ELSE = "else";
const char* STATEMENT_RETURN = "return";
const char* STATEMENT_NEW = "new";
const char* STATEMENT_ITERATE = "iterate";

const char* VALUE_FALSE = "false";
const char* VALUE_TRUE = "true";
const char* VALUE_NULL = "NULL";

hermes_parser_T* init_hermes_parser(hermes_lexer_T* hermes_lexer)
{
    hermes_parser_T* hermes_parser = calloc(1, sizeof(struct HERMES_PARSER_STRUCT));
    hermes_parser->hermes_lexer = hermes_lexer;
    hermes_parser->current_token = hermes_lexer_get_next_token(hermes_parser->hermes_lexer);
    hermes_parser->prev_token = (void*)0;

    return hermes_parser;
}

// etc

void hermes_parser_type_error(hermes_parser_T* hermes_parser)
{
    printf("[Line %d] Invalid type for assigned value\n", hermes_parser->hermes_lexer->line_n);
    exit(1);
}

AST_T* hermes_parser_parse(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    return hermes_parser_parse_statements(hermes_parser, scope);
}

AST_T* hermes_parser_eat(hermes_parser_T* hermes_parser, int token_type)
{
    if (hermes_parser->current_token->type != token_type)
    {
        printf(
            "[Line %d] Unexpected token `%s`, was expecting `%d`.\n",
            hermes_parser->hermes_lexer->line_n,
            hermes_parser->current_token->value,
            token_type
        );
        exit(1);
    }
    else if (hermes_parser->current_token->type == token_type)
    {
        if (hermes_parser->prev_token)
            token_free(hermes_parser->prev_token);

        hermes_parser->prev_token = hermes_parser->current_token;
        hermes_parser->current_token = hermes_lexer_get_next_token(hermes_parser->hermes_lexer);
    }

    return (void*)0;
}

AST_T* hermes_parser_parse_statement(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    switch (hermes_parser->current_token->type)
    {
        case TOKEN_ID: {
            char* token_value = hermes_parser->current_token->value;

            if (strcmp(token_value, STATEMENT_WHILE) == 0)
                return hermes_parser_parse_while(hermes_parser, scope);

            if (strcmp(token_value, STATEMENT_IF) == 0)
                return hermes_parser_parse_if(hermes_parser, scope);

            if (strcmp(token_value, VALUE_FALSE) == 0 || strcmp(token_value, VALUE_TRUE) == 0)
                return hermes_parser_parse_boolean(hermes_parser, scope);

            if (strcmp(token_value, VALUE_NULL) == 0)
                return hermes_parser_parse_null(hermes_parser, scope);

            if (strcmp(token_value, STATEMENT_RETURN) == 0)
                return hermes_parser_parse_return(hermes_parser, scope);

            if (strcmp(token_value, STATEMENT_NEW) == 0)
                return hermes_parser_parse_new(hermes_parser, scope);

            if (strcmp(token_value, STATEMENT_ITERATE) == 0)
                return hermes_parser_parse_iterate(hermes_parser, scope);

            if (
                strcmp(token_value, DATA_TYPE_VOID) == 0 ||
                strcmp(token_value, DATA_TYPE_INT) == 0 ||
                strcmp(token_value, DATA_TYPE_STRING) == 0 ||
                strcmp(token_value, DATA_TYPE_CHAR) == 0 ||
                strcmp(token_value, DATA_TYPE_FLOAT) == 0 ||
                strcmp(token_value, DATA_TYPE_BOOLEAN) == 0 ||
                strcmp(token_value, DATA_TYPE_OBJECT) == 0 ||
                strcmp(token_value, DATA_TYPE_LIST) == 0 ||
                strcmp(token_value, DATA_TYPE_SOURCE) == 0 ||
                strcmp(token_value, DATA_TYPE_REFERENCE) == 0
            )
                return hermes_parser_parse_function_definition(hermes_parser, scope);

            hermes_parser_eat(hermes_parser, TOKEN_ID);

            AST_T* a = (void*) 0;

            if (hermes_parser->current_token->type == TOKEN_LPAREN)
            {
                a = hermes_parser_parse_function_call(hermes_parser, scope);
            }
            else
            {
                a = hermes_parser_parse_variable(hermes_parser, scope);
            }

            while (hermes_parser->current_token->type == TOKEN_DOT)
            {
                hermes_parser_eat(hermes_parser, TOKEN_DOT);
                AST_T* ast = init_ast(AST_ATTRIBUTE_ACCESS);
                ast->binop_left = a;
                ast->binop_right = hermes_parser_parse_expr(hermes_parser, scope);

                a = ast;
            }

            while (hermes_parser->current_token->type == TOKEN_LBRACKET)
            {
                AST_T* ast_list_access = init_ast(AST_LIST_ACCESS);
                ast_list_access->binop_left = a;
                hermes_parser_eat(hermes_parser, TOKEN_LBRACKET);
                ast_list_access->list_access_pointer = hermes_parser_parse_expr(hermes_parser, scope);
                hermes_parser_eat(hermes_parser, TOKEN_RBRACKET);

                a = ast_list_access;
            }

            if (a)
                return a;

        } break;
        case TOKEN_NUMBER_VALUE: case TOKEN_STRING_VALUE: case TOKEN_CHAR_VALUE: case TOKEN_FLOAT_VALUE: case TOKEN_INTEGER_VALUE: return hermes_parser_parse_expr(hermes_parser, scope); break;
        default: return init_ast(AST_NOOP); break;
    }

    return init_ast(AST_NOOP);
}

AST_T* hermes_parser_parse_statements(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->scope = (struct hermes_scope_T*) scope;

    AST_T* statement = hermes_parser_parse_statement(hermes_parser, scope);

    dynamic_list_append(compound->compound_value, statement);

    while (hermes_parser->current_token->type == TOKEN_SEMI || statement->type != AST_NOOP)
    {
        if (hermes_parser->current_token->type == TOKEN_SEMI)
            hermes_parser_eat(hermes_parser, TOKEN_SEMI);

        statement = hermes_parser_parse_statement(hermes_parser, scope);

        dynamic_list_append(compound->compound_value, statement);
    }

    return compound;
}

AST_T* hermes_parser_parse_type(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_type = init_ast(AST_TYPE);
    ast_type->scope = (struct hermes_scope_T*) scope;
    ast_type->type_value = calloc(strlen(hermes_parser->current_token->value) + 1, sizeof(char));
    strcpy(ast_type->type_value, hermes_parser->current_token->value);

    hermes_parser_eat(hermes_parser, TOKEN_ID);

    return ast_type;
}

AST_T* hermes_parser_parse_attribute_access(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    return init_ast(AST_NOOP);
}

// values

AST_T* hermes_parser_parse_float(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_float = init_ast(AST_FLOAT);
    ast_float->scope = (struct hermes_scope_T*) scope;
    ast_float->float_value = (float) atof(hermes_parser->current_token->value);

    hermes_parser_eat(hermes_parser, TOKEN_FLOAT_VALUE);

    return ast_float;
}

AST_T* hermes_parser_parse_string(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->scope = (struct hermes_scope_T*) scope;
    ast_string->string_value = calloc(strlen(hermes_parser->current_token->value) + 1, sizeof(char));
    strcpy(ast_string->string_value, hermes_parser->current_token->value);

    hermes_parser_eat(hermes_parser, TOKEN_STRING_VALUE);

    return ast_string;
}

AST_T* hermes_parser_parse_char(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_string = init_ast(AST_CHAR);
    ast_string->scope = (struct hermes_scope_T*) scope;
    ast_string->char_value = hermes_parser->current_token->value[0];

    hermes_parser_eat(hermes_parser, TOKEN_CHAR_VALUE);

    return ast_string;
}

AST_T* hermes_parser_parse_integer(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_integer = init_ast(AST_INTEGER);
    ast_integer->scope = (struct hermes_scope_T*) scope;
    ast_integer->int_value = atoi(hermes_parser->current_token->value);

    hermes_parser_eat(hermes_parser, TOKEN_INTEGER_VALUE);

    return ast_integer;
}

AST_T* hermes_parser_parse_array(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    return init_ast(AST_NOOP);
}

AST_T* hermes_parser_parse_boolean(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_boolean = init_ast(AST_BOOLEAN);
    ast_boolean->scope = (struct hermes_scope_T*) scope;
    
    if (strcmp(hermes_parser->current_token->value, "false") == 0)
        ast_boolean->boolean_value = 0; 
    else if (strcmp(hermes_parser->current_token->value, "true") == 0)
        ast_boolean->boolean_value = 1;
    else
        printf("%s is not a boolean value\n");

    hermes_parser_eat(hermes_parser, TOKEN_ID);

    return ast_boolean;
}

AST_T* hermes_parser_parse_null(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_null = init_ast(AST_NULL);
    ast_null->scope = (struct hermes_scope_T*) scope;

    hermes_parser_eat(hermes_parser, TOKEN_ID);

    return ast_null;
}

AST_T* hermes_parser_parse_variable(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->scope = (struct hermes_scope_T*) scope;
    ast_variable->variable_name = calloc(strlen(hermes_parser->prev_token->value) + 1, sizeof(char));
    strcpy(ast_variable->variable_name, hermes_parser->prev_token->value);

    if (hermes_parser->current_token->type == TOKEN_EQUALS)
    {
        hermes_parser_eat(hermes_parser, TOKEN_EQUALS);
        AST_T* ast_assign = init_ast(AST_VARIABLE_ASSIGNMENT);
        ast_assign->variable_assignment_left = ast_variable;
        ast_assign->variable_value = hermes_parser_parse_expr(hermes_parser, scope);
        ast_assign->scope = (struct hermes_scope_T*) scope;

        return ast_assign;
    }

    if (
        hermes_parser->current_token->type == TOKEN_PLUS_EQUALS ||
        hermes_parser->current_token->type == TOKEN_MINUS_EQUALS ||
        hermes_parser->current_token->type == TOKEN_STAR_EQUALS
    )
    {
        int type = hermes_parser->current_token->type;

        hermes_parser_eat(hermes_parser, type);
        AST_T* ast_variable_modifier = init_ast(AST_VARIABLE_MODIFIER);
        ast_variable_modifier->binop_left = ast_variable;
        ast_variable_modifier->binop_right = hermes_parser_parse_expr(hermes_parser, scope);
        ast_variable_modifier->binop_operator = type;
        ast_variable_modifier->scope = (struct hermes_scope_T*) scope;

        return ast_variable_modifier;
    }

    return ast_variable;
}

AST_T* hermes_parser_parse_object(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_object = init_ast(AST_OBJECT);
    ast_object->scope = (struct hermes_scope_T*) scope;
    ast_object->object_children = init_dynamic_list(sizeof(struct AST_STRUCT));
    hermes_scope_T* new_scope = init_hermes_scope(0);

    if (scope)
        if (scope->owner)
            new_scope->owner = scope->owner;

    hermes_parser_eat(hermes_parser, TOKEN_LBRACE);

    if (hermes_parser->current_token->type != TOKEN_RBRACE)
    {
        if (hermes_parser->current_token->type == TOKEN_ID)
            dynamic_list_append(ast_object->object_children, hermes_parser_parse_function_definition(hermes_parser, new_scope));

        while (hermes_parser->current_token->type == TOKEN_SEMI)
        {
            hermes_parser_eat(hermes_parser, TOKEN_SEMI);

            if (hermes_parser->current_token->type == TOKEN_ID)
                dynamic_list_append(ast_object->object_children, hermes_parser_parse_function_definition(hermes_parser, new_scope));
        }
    }
    
    hermes_parser_eat(hermes_parser, TOKEN_RBRACE);

    return ast_object;
}

AST_T* hermes_parser_parse_list(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    hermes_parser_eat(hermes_parser, TOKEN_LBRACKET);
    AST_T* ast_list = init_ast(AST_LIST);
    ast_list->scope = (struct hermes_scope_T*) scope;
    ast_list->list_children = init_dynamic_list(sizeof(struct AST_STRUCT));

    if (hermes_parser->current_token->type != TOKEN_RBRACKET)
        dynamic_list_append(ast_list->list_children, hermes_parser_parse_expr(hermes_parser, scope));

    while (hermes_parser->current_token->type == TOKEN_COMMA)
    {
        hermes_parser_eat(hermes_parser, TOKEN_COMMA);
        dynamic_list_append(ast_list->list_children, hermes_parser_parse_expr(hermes_parser, scope));
    }
    
    hermes_parser_eat(hermes_parser, TOKEN_RBRACKET);

    return ast_list;
}

// math

AST_T* hermes_parser_parse_factor(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    if (strcmp(hermes_parser->current_token->value, VALUE_FALSE) == 0 || strcmp(hermes_parser->current_token->value, VALUE_TRUE) == 0)
        return hermes_parser_parse_boolean(hermes_parser, scope);

    if (strcmp(hermes_parser->current_token->value, VALUE_NULL) == 0)
        return hermes_parser_parse_null(hermes_parser, scope);

    if (strcmp(hermes_parser->current_token->value, STATEMENT_NEW) == 0)
        return hermes_parser_parse_new(hermes_parser, scope);

    if (hermes_parser->current_token->type == TOKEN_ID)
    {
        hermes_parser_eat(hermes_parser, TOKEN_ID);

        AST_T* a = (void*) 0;

        switch (hermes_parser->current_token->type)
        {
            case TOKEN_LPAREN: a = hermes_parser_parse_function_call(hermes_parser, scope); break;
            default: a = hermes_parser_parse_variable(hermes_parser, scope); break;
        }

        if (hermes_parser->current_token->type == TOKEN_DOT)
        {
            hermes_parser_eat(hermes_parser, TOKEN_DOT);
            AST_T* ast = init_ast(AST_ATTRIBUTE_ACCESS);
            ast->binop_left = a;
            ast->binop_right = hermes_parser_parse_factor(hermes_parser, scope);

            a = ast;
        }

        while (hermes_parser->current_token->type == TOKEN_LBRACKET)
        {
            AST_T* ast_list_access = init_ast(AST_LIST_ACCESS);
            ast_list_access->binop_left = a;
            hermes_parser_eat(hermes_parser, TOKEN_LBRACKET);
            ast_list_access->list_access_pointer = hermes_parser_parse_expr(hermes_parser, scope);
            hermes_parser_eat(hermes_parser, TOKEN_RBRACKET);

            a = ast_list_access;
        }

        if (a)
            return a;
    }

   if (hermes_parser->current_token->type == TOKEN_LPAREN)
   {
       hermes_parser_eat(hermes_parser, TOKEN_LPAREN);
       AST_T* ast_expr = hermes_parser_parse_expr(hermes_parser, scope);
       hermes_parser_eat(hermes_parser, TOKEN_RPAREN);

       return ast_expr;
   } 

    switch (hermes_parser->current_token->type)
    {
        case TOKEN_NUMBER_VALUE: return hermes_parser_parse_integer(hermes_parser, scope); break;
        case TOKEN_INTEGER_VALUE: return hermes_parser_parse_integer(hermes_parser, scope); break;
        case TOKEN_FLOAT_VALUE: return hermes_parser_parse_float(hermes_parser, scope); break;
        case TOKEN_STRING_VALUE: return hermes_parser_parse_string(hermes_parser, scope); break;
        case TOKEN_CHAR_VALUE: return hermes_parser_parse_char(hermes_parser, scope); break;
        case TOKEN_LBRACE: return hermes_parser_parse_object(hermes_parser, scope); break;
        case TOKEN_LBRACKET: return hermes_parser_parse_list(hermes_parser, scope); break;
        default: return hermes_parser_parse_expr(hermes_parser, scope); break;
    }
}

AST_T* hermes_parser_parse_term(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    char* token_value = hermes_parser->current_token->value;

    if (
        strcmp(token_value, DATA_TYPE_VOID) == 0 ||
        strcmp(token_value, DATA_TYPE_INT) == 0 ||
        strcmp(token_value, DATA_TYPE_STRING) == 0 ||
        strcmp(token_value, DATA_TYPE_CHAR) == 0 ||
        strcmp(token_value, DATA_TYPE_FLOAT) == 0 ||
        strcmp(token_value, DATA_TYPE_BOOLEAN) == 0 ||
        strcmp(token_value, DATA_TYPE_OBJECT) == 0 ||
        strcmp(token_value, DATA_TYPE_LIST) == 0 ||
        strcmp(token_value, DATA_TYPE_SOURCE) == 0 ||
        strcmp(token_value, DATA_TYPE_REFERENCE) == 0
    ) // this is to be able to have variable definitions inside of function definition parantheses.
        return hermes_parser_parse_function_definition(hermes_parser, scope);

    AST_T* node = hermes_parser_parse_factor(hermes_parser, scope);
    AST_T* ast_binop = (void*) 0;

    while (
        hermes_parser->current_token->type == TOKEN_DIV ||        
        hermes_parser->current_token->type == TOKEN_STAR ||
        hermes_parser->current_token->type == TOKEN_AND ||
        hermes_parser->current_token->type == TOKEN_LESS_THAN ||
        hermes_parser->current_token->type == TOKEN_LARGER_THAN ||
        hermes_parser->current_token->type == TOKEN_EQUALS_EQUALS ||
        hermes_parser->current_token->type == TOKEN_NOT_EQUALS
    )
    {
        int binop_operator = hermes_parser->current_token->type;
        hermes_parser_eat(hermes_parser, hermes_parser->current_token->type);

        ast_binop = init_ast(AST_BINOP);
        
        ast_binop->binop_left = node;
        ast_binop->binop_operator = binop_operator;
        ast_binop->binop_right = hermes_parser_parse_factor(hermes_parser, scope);

        node = ast_binop;
    }
    
    return node;
}

AST_T* hermes_parser_parse_expr(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* node = hermes_parser_parse_term(hermes_parser, scope);
    AST_T* ast_binop = (void*) 0;

    while (
        hermes_parser->current_token->type == TOKEN_PLUS ||
        hermes_parser->current_token->type == TOKEN_MINUS
    )
    {
        int binop_operator = hermes_parser->current_token->type;
        hermes_parser_eat(hermes_parser, hermes_parser->current_token->type);

        ast_binop = init_ast(AST_BINOP);
        ast_binop->scope = (struct hermes_scope_T*) scope;

        ast_binop->binop_left = node;
        ast_binop->binop_operator = binop_operator;
        ast_binop->binop_right = hermes_parser_parse_term(hermes_parser, scope);

        node = ast_binop;
    }

    return node;
}

// statements

AST_T* hermes_parser_parse_break(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    return init_ast(AST_BREAK);
}

AST_T* hermes_parser_parse_return(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    hermes_parser_eat(hermes_parser, TOKEN_ID);
    AST_T* ast_return = init_ast(AST_RETURN);
    ast_return->scope = (struct hermes_scope_T*) scope;

    if (hermes_parser->current_token->type != TOKEN_SEMI)
        ast_return->return_value = hermes_parser_parse_expr(hermes_parser, scope);

    return ast_return;
}

AST_T* hermes_parser_parse_if(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_if = init_ast(AST_IF);
    
    if (hermes_parser->current_token->type == TOKEN_ID) {
        hermes_parser_eat(hermes_parser, TOKEN_ID);
    }
    
    if (hermes_parser->current_token->type == TOKEN_LPAREN)
    {
        hermes_parser_eat(hermes_parser, TOKEN_LPAREN);
        ast_if->if_expr = hermes_parser_parse_expr(hermes_parser, scope);
        hermes_parser_eat(hermes_parser, TOKEN_RPAREN);
    }

    hermes_parser_eat(hermes_parser, TOKEN_LBRACE);
    ast_if->if_body = hermes_parser_parse_statements(hermes_parser, scope);
    ast_if->scope = (struct hermes_scope_T*) scope;
    hermes_parser_eat(hermes_parser, TOKEN_RBRACE);

    if (strcmp(hermes_parser->current_token->value, STATEMENT_ELSE) == 0)
    {
        hermes_parser_eat(hermes_parser, TOKEN_ID);
        ast_if->if_otherwise = hermes_parser_parse_if(hermes_parser, scope);
        ast_if->if_otherwise->scope = (struct hermes_scope_T*) scope;
    }

    return ast_if;
}

AST_T* hermes_parser_parse_new(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    hermes_parser_eat(hermes_parser, TOKEN_ID);
    AST_T* ast_new = init_ast(AST_NEW);
    ast_new->new_value = hermes_parser_parse_expr(hermes_parser, scope);

    return ast_new;
}


AST_T* hermes_parser_parse_iterate(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    hermes_parser_eat(hermes_parser, TOKEN_ID); // iterate
    hermes_parser_eat(hermes_parser, TOKEN_ID);
    AST_T* ast_var = hermes_parser_parse_variable(hermes_parser, scope); // variable
    hermes_parser_eat(hermes_parser, TOKEN_ID); // with
    hermes_parser_eat(hermes_parser, TOKEN_ID);
    AST_T* ast_fname = hermes_parser_parse_variable(hermes_parser, scope); // function name

    AST_T* ast_iterate = init_ast(AST_ITERATE);
    ast_iterate->iterate_iterable = ast_var;
    ast_iterate->iterate_function = ast_fname;

    return ast_iterate;
}

// loops

AST_T* hermes_parser_parse_while(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    hermes_parser_eat(hermes_parser, TOKEN_ID);
    hermes_parser_eat(hermes_parser, TOKEN_LPAREN);
    AST_T* ast_while = init_ast(AST_WHILE);
    ast_while->while_expr = hermes_parser_parse_expr(hermes_parser, scope);
    hermes_parser_eat(hermes_parser, TOKEN_RPAREN);
    hermes_parser_eat(hermes_parser, TOKEN_LBRACE);
    ast_while->while_body = hermes_parser_parse_statements(hermes_parser, scope);
    hermes_parser_eat(hermes_parser, TOKEN_RBRACE);
    ast_while->scope = (struct hermes_scope_T*) scope;

    return ast_while;
}

// functions

AST_T* hermes_parser_parse_function_call(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_function_call = init_ast(AST_FUNCTION_CALL);
    ast_function_call->function_call_name = calloc(strlen(hermes_parser->prev_token->value) + 1, sizeof(char));
    strcpy(ast_function_call->function_call_name, hermes_parser->prev_token->value);
    hermes_parser_eat(hermes_parser, TOKEN_LPAREN);
    ast_function_call->scope = (struct hermes_scope_T*) scope;

    if (hermes_parser->current_token->type != TOKEN_RPAREN)
    {
        dynamic_list_append(ast_function_call->function_call_arguments, hermes_parser_parse_expr(hermes_parser, scope));

        while (hermes_parser->current_token->type == TOKEN_COMMA)
        {
            hermes_parser_eat(hermes_parser, TOKEN_COMMA);
            dynamic_list_append(ast_function_call->function_call_arguments, hermes_parser_parse_expr(hermes_parser, scope));
        }
    }

    hermes_parser_eat(hermes_parser, TOKEN_RPAREN);

    return ast_function_call;
}

AST_T* hermes_parser_parse_function_definition(hermes_parser_T* hermes_parser, hermes_scope_T* scope)
{
    AST_T* ast_type = hermes_parser_parse_type(hermes_parser, scope);
    char* function_name = calloc(strlen(hermes_parser->current_token->value) + 1, sizeof(char));
    strcpy(function_name, hermes_parser->current_token->value);
    hermes_parser_eat(hermes_parser, TOKEN_ID);

    if (hermes_parser->current_token->type == TOKEN_LPAREN)
    {
        // alright, it sure is a function definition!
        
        AST_T* ast_function_definition = init_ast(AST_FUNCTION_DEFINITION);
        hermes_scope_T* new_scope = init_hermes_scope(0);
        new_scope->owner = ast_function_definition;

        ast_function_definition->function_name = function_name;
        ast_function_definition->function_definition_type = ast_type;
        ast_function_definition->function_definition_arguments = init_dynamic_list(sizeof(struct AST_STRUCT));

        hermes_parser_eat(hermes_parser, TOKEN_LPAREN);

        if (hermes_parser->current_token->type != TOKEN_RPAREN)
        {
            dynamic_list_append(ast_function_definition->function_definition_arguments, hermes_parser_parse_expr(hermes_parser, new_scope));

            while (hermes_parser->current_token->type == TOKEN_COMMA)
            {
                hermes_parser_eat(hermes_parser, TOKEN_COMMA);
                dynamic_list_append(ast_function_definition->function_definition_arguments, hermes_parser_parse_expr(hermes_parser, new_scope));
            }
        } 

        hermes_parser_eat(hermes_parser, TOKEN_RPAREN);

        if (hermes_parser->current_token->type == TOKEN_EQUALS)
        {
            hermes_parser_eat(hermes_parser, TOKEN_EQUALS);
            hermes_parser_eat(hermes_parser, TOKEN_ID);

            AST_T* child_def = hermes_parser_parse_variable(hermes_parser, scope);

            child_def->scope = (struct hermes_scope_T*) new_scope;
            dynamic_list_append(ast_function_definition->composition_children, child_def);

            while (hermes_parser->current_token->type == TOKEN_COMMA)
            {
                hermes_parser_eat(hermes_parser, TOKEN_COMMA);

                hermes_parser_eat(hermes_parser, TOKEN_ID);
                child_def = hermes_parser_parse_variable(hermes_parser, scope);
                child_def->scope = (struct hermes_scope_T*) new_scope;
                dynamic_list_append(ast_function_definition->composition_children, child_def);
            }

            return ast_function_definition;
        }

        hermes_parser_eat(hermes_parser, TOKEN_LBRACE);
        ast_function_definition->function_definition_body = hermes_parser_parse_statements(hermes_parser, new_scope);
        ast_function_definition->function_definition_body->scope = (struct hermes_scope_T*) new_scope;
        hermes_parser_eat(hermes_parser, TOKEN_RBRACE);

        return ast_function_definition;
    }
    else
    {
        // alright, it is a variable definition.
        AST_T* ast_variable_definition = init_ast(AST_VARIABLE_DEFINITION);
        ast_variable_definition->scope = (struct hermes_scope_T*) scope;
        ast_variable_definition->variable_name = function_name;
        ast_variable_definition->variable_type = ast_type;
        
        if (hermes_parser->current_token->type == TOKEN_EQUALS)
        {
            hermes_parser_eat(hermes_parser, TOKEN_EQUALS);
            ast_variable_definition->variable_value = hermes_parser_parse_expr(hermes_parser, scope);

            /**
             * Performing all the kinds of type-checks we can possibly cover,
             * this is only for reducing type-errors, we cannot possibly cover
             * everything here since the value might be kind of unknown, but
             * this is better than nothing.
             */
            switch(ast_variable_definition->variable_value->type)
            {
                case AST_OBJECT: if (strcmp(ast_type->type_value, DATA_TYPE_OBJECT) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_STRING: if (strcmp(ast_type->type_value, DATA_TYPE_STRING) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_INTEGER: if (strcmp(ast_type->type_value, DATA_TYPE_INT) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_FLOAT: if (strcmp(ast_type->type_value, DATA_TYPE_FLOAT) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_BOOLEAN: if (strcmp(ast_type->type_value, DATA_TYPE_BOOLEAN) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_LIST: if (strcmp(ast_type->type_value, DATA_TYPE_LIST) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_CHAR: if (strcmp(ast_type->type_value, DATA_TYPE_CHAR) != 0) hermes_parser_type_error(hermes_parser); break;
                case AST_COMPOUND: if (strcmp(ast_type->type_value, DATA_TYPE_SOURCE) != 0) hermes_parser_type_error(hermes_parser); break;
            }
        }

        return ast_variable_definition;
    }
}
