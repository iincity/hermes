#include "include/AST.h"
#include "include/hermes_scope.h"
#include <stdlib.h>
#include <string.h>


AST_T* init_ast(int type)
{
    AST_T* AST = calloc(1, sizeof(struct AST_STRUCT));
    AST->type = type;
    AST->function_call_name = (void*) 0;
    AST->int_value = 0;
    AST->boolean_value = 0;
    AST->is_object_child = 0;
    AST->float_value = 0;
    AST->char_value = '\0';
    AST->string_value = (void*) 0;
    AST->type_value = (void*) 0;
    AST->variable_name = (void*) 0;
    AST->variable_value = (void*) 0;
    AST->variable_type = (void*) 0;
    AST->variable_assignment_left = (void*) 0;
    AST->function_name = (void*) 0;
    AST->binop_left = (void*) 0;
    AST->binop_right = (void*) 0;
    AST->binop_operator;
    AST->compound_value = AST->type == AST_COMPOUND ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->function_call_arguments = AST->type == AST_FUNCTION_CALL ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->function_definition_arguments = AST->type == AST_FUNCTION_DEFINITION ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->object_children = AST->type == AST_OBJECT ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->object_value = (void*) 0;
    AST->list_children = AST->type == AST_LIST ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->function_definitions = (void*) 0;
    AST->composition_children = AST->type == AST_FUNCTION_DEFINITION ? init_dynamic_list(sizeof(struct AST_STRUCT*)) : (void*) 0;
    AST->function_definition_body = (void*) 0;
    AST->function_definition_type = (void*) 0;
    AST->if_expr = (void*) 0;
    AST->if_body = (void*) 0;
    AST->if_otherwise = (void*) 0;
    AST->else_expr = (void*) 0;
    AST->while_expr = (void*) 0;
    AST->while_body = (void*) 0;
    AST->return_value = (void*) 0;
    AST->list_access_pointer = (void*) 0;
    AST->saved_function_call = (void*) 0;
    AST->new_value = (void*) 0;
    AST->scope = (void*) 0;
    AST->fptr = (void*) 0;

    return AST;
}

void _ast_free(void* ast)
{
    AST_T* a = (AST_T*) ast;
    ast_free(a);
}

void ast_free(AST_T* ast)
{
    if (ast == (void*) 0)
        return;

    // these are free'd else where.
    if (ast->type == AST_FUNCTION_DEFINITION || ast->type == AST_VARIABLE_DEFINITION)
        return;

    if (ast->function_call_name)
        free(ast->function_call_name);

    if (ast->string_value)
        free(ast->string_value);

    if (ast->type_value)
        free(ast->type_value);

    if (ast->variable_name)
        free(ast->variable_name);

    if (ast->variable_value)
        ast_free(ast->variable_value);

    if (ast->variable_type)
        ast_free(ast->variable_type);

    if (ast->variable_assignment_left)
        ast_free(ast->variable_assignment_left);

    if (ast->function_name)
        free(ast->function_name);

    if (ast->binop_left)
        ast_free(ast->binop_left);

    if (ast->binop_right)
        ast_free(ast->binop_right);

    //if (ast->function_definition_body)
    //    ast_free(ast->function_definition_body);

    if (ast->compound_value)
    {
        for (int i = 0; i < ast->compound_value->size; i++)
        {
            if ((AST_T*)ast->compound_value->items[i] != ast)
                ast_free((AST_T*)ast->compound_value->items[i]);
        }

        free(ast->compound_value->items);
        free(ast->compound_value);
        ast->compound_value = (void*) 0;
    }

    if (ast->function_call_arguments)
    {
        for (int i = 0; i < ast->function_call_arguments->size; i++)
        {
            if ((AST_T*)ast->function_call_arguments->items[i] != ast)
                ast_free((AST_T*)ast->function_call_arguments->items[i]);
        }

        free(ast->function_call_arguments->items);
        free(ast->function_call_arguments);
        ast->function_call_arguments = (void*) 0;
    }

    if (ast->function_definition_arguments)
    {
        for (int i = 0; i < ast->function_definition_arguments->size; i++)
        {
            if ((AST_T*)ast->function_definition_arguments->items[i] != ast)
                ast_free((AST_T*)ast->function_definition_arguments->items[i]);
        }

        free(ast->function_definition_arguments->items);
        free(ast->function_definition_arguments);
        ast->function_definition_arguments = (void*) 0;
    }

    if (ast->object_children)
    {
        // TODO: free
    }

    if (ast->object_value)
    {
        // hmm ...
    }

    if (ast->list_children)
    {
        // TODO: free
    }

    if (ast->function_definitions)
    {
        //free(ast->function_definitions->items);
        //free(ast->function_definitions);
        //ast->function_definitions = (void*) 0;
    }

    if (ast->composition_children)
    {
        for (int i = 0; i < ast->composition_children->size; i++)
        {
            if ((AST_T*)ast->composition_children->items[i] != ast)
                ast_free((AST_T*)ast->composition_children->items[i]);
        }

        free(ast->composition_children->items);
        free(ast->composition_children);
        ast->composition_children = (void*) 0;
    } 

    if (ast->function_definition_type)
        ast_free(ast->function_definition_type);

    if (ast->if_expr)
        ast_free(ast->if_expr);

    if (ast->if_body)
        ast_free(ast->if_body);

    if (ast->if_otherwise)
        ast_free(ast->if_otherwise);

    if (ast->else_expr)
        ast_free(ast->else_expr);

    if (ast->while_expr)
        ast_free(ast->while_expr);

    if (ast->while_body)
        ast_free(ast->while_body);

    if (ast->return_value)
        ast_free(ast->return_value);

    if (ast->list_access_pointer)
        ast_free(ast->list_access_pointer);

    if (ast->new_value)
        ast_free(ast->new_value);

    /*if (ast->scope)
    {
        hermes_scope_T* scope = (hermes_scope_T*) ast->scope;

        if (!scope->global)
        {
            hermes_scope_free(scope);
            scope = (void*) 0;
            ast->scope = (void*)0;
        }
    }*/

    free(ast);
}

AST_T* ast_copy_object(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->object_children = init_dynamic_list(sizeof(struct AST_STRUCT*));
    
    for (int i = 0; i < ast->object_children->size; i++)
    {
        AST_T* child_copy = ast_copy(ast->object_children->items[i]);
        dynamic_list_append(a->object_children, child_copy);
    }

    return a;
}

AST_T* ast_copy_variable(AST_T* ast)
{
    AST_T* type;
    
    if (ast->variable_type)
        type = ast_copy(ast->variable_type);

    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->variable_type = type;
    a->variable_value = ast_copy(ast->variable_value);
    a->variable_name = calloc(strlen(ast->variable_name) + 1, sizeof(char));
    strcpy(a->variable_name, ast->variable_name);

    return a;
}

AST_T* ast_copy_variable_definition(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->variable_value = ast_copy(ast->variable_value);
    a->variable_type = ast_copy(ast->variable_type);
    a->variable_name = calloc(strlen(ast->variable_name) + 1, sizeof(char));
    strcpy(a->variable_name, ast->variable_name);

    return a;
}

AST_T* ast_copy_function_definition(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->function_name = calloc(strlen(ast->function_name) + 1, sizeof(char));
    strcpy(a->function_name, ast->function_name);
    a->function_definition_body = ast_copy(ast->function_definition_body);
    a->function_definition_arguments = init_dynamic_list(sizeof(struct AST_STRUCT*));

    for (int i = 0; i < ast->function_definition_arguments->size; i++)
    {
        AST_T* child_copy = ast_copy(ast->function_definition_arguments->items[i]);
        dynamic_list_append(a->function_definition_arguments, child_copy);
    }

    return a;
}

AST_T* ast_copy_string(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->string_value = calloc(strlen(ast->string_value) + 1, sizeof(char));
    strcpy(a->string_value, ast->string_value);

    return a;
}

AST_T* ast_copy_char(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->char_value = ast->char_value;

    return a;
}

AST_T* ast_copy_float(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->float_value = ast->float_value;

    return a;
}

AST_T* ast_copy_list(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->list_children = init_dynamic_list(sizeof(struct AST_STRUCT*));
    
    for (int i = 0; i < ast->list_children->size; i++)
    {
        AST_T* child_copy = ast_copy(ast->list_children->items[i]);
        dynamic_list_append(a->list_children, child_copy);
    }

    return a;
}

AST_T* ast_copy_boolean(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->boolean_value = ast->boolean_value;

    return a;
}

AST_T* ast_copy_integer(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->int_value = ast->int_value;

    return a;
}

AST_T* ast_copy_compound(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->compound_value = init_dynamic_list(sizeof(struct AST_STRUCT*));

    for (int i = 0; i < ast->compound_value->size; i++)
    {
        AST_T* child_copy = ast_copy(ast->compound_value->items[i]);
        dynamic_list_append(a->compound_value, child_copy);
    }

    return a;
}

AST_T* ast_copy_type(AST_T* ast)
{
    AST_T* type = init_ast(ast->type);
    type->scope = ast->scope;
    type->type_value = calloc(strlen(ast->type_value) + 1, sizeof(char));
    strcpy(type->type_value, ast->type_value);

    return type;
}

AST_T* ast_copy_attribute_access(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    a->binop_left = ast_copy(ast->binop_left);
    a->binop_right = ast_copy(ast->binop_right);
    a->binop_operator = ast->binop_operator;

    return a;
}

AST_T* ast_copy_return(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->scope = ast->scope;
    
    if (ast->return_value)
        a->return_value = ast_copy(ast->return_value);

    return a;
}

AST_T* ast_copy_variable_assignment(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->variable_assignment_left = ast_copy(ast->variable_assignment_left);
    a->variable_value = ast_copy(ast->variable_value);

    return a;
}

AST_T* ast_copy_variable_modifier(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->binop_left = ast_copy(ast->binop_left);
    a->binop_right = ast_copy(ast->binop_right);
    a->binop_operator = ast->binop_operator;

    return a;
}

AST_T* ast_copy_function_call(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->function_call_name = calloc(strlen(ast->function_call_name) + 1, sizeof(char));
    strcpy(a->function_call_name, ast->function_call_name);

    for (int i = 0; i < ast->function_call_arguments->size; i++)
    {
        AST_T* child_copy = ast_copy(ast->function_call_arguments->items[i]);
        dynamic_list_append(a->function_call_arguments, child_copy);
    }

    return a;
}

AST_T* ast_copy_null(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);

    return a;
}

AST_T* ast_copy_list_access(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->list_access_pointer = ast_copy(a->list_access_pointer);

    return a;
}

AST_T* ast_copy_binop(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->binop_left = ast_copy(ast->binop_left);
    a->binop_operator = ast->binop_operator;
    a->binop_right = ast_copy(ast->binop_right);

    return a;
}

AST_T* ast_copy_if(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->if_expr = ast_copy(ast->if_expr);
    a->if_body = ast_copy(ast->if_body);
    a->if_otherwise = ast_copy(ast->if_otherwise);

    return a;
}

AST_T* ast_copy_while(AST_T* ast)
{
    AST_T* a = init_ast(ast->type);
    a->while_expr = ast_copy(ast->while_expr);
    a->while_body = ast_copy(ast->while_body);

    return a;
}

static AST_T* ast_r_scope(AST_T* ast, struct hermes_scope_T* scope)
{
    ast->scope = scope;
    return ast;
}

AST_T* ast_copy(AST_T* ast)
{
    if (!ast)
        return (void*)0;

    switch (ast->type)
    {
        case AST_OBJECT: return ast_r_scope(ast_copy_object(ast), ast->scope); break;
        case AST_VARIABLE: return ast_r_scope(ast_copy_variable(ast), ast->scope); break;
        case AST_VARIABLE_DEFINITION: return ast_r_scope(ast_copy_variable_definition(ast), ast->scope); break;
        case AST_VARIABLE_ASSIGNMENT: return ast_r_scope(ast_copy_variable_assignment(ast), ast->scope); break;
        case AST_VARIABLE_MODIFIER: return ast_r_scope(ast_copy_variable_modifier(ast), ast->scope); break;
        case AST_FUNCTION_DEFINITION: return ast_r_scope(ast_copy_function_definition(ast), ast->scope); break;
        case AST_FUNCTION_CALL: return ast_r_scope(ast_copy_function_call(ast), ast->scope); break;
        case AST_NULL: return ast_r_scope(ast_copy_null(ast), ast->scope); break;
        case AST_STRING: return ast_r_scope(ast_copy_string(ast), ast->scope); break;
        case AST_CHAR: return ast_r_scope(ast_copy_char(ast), ast->scope); break;
        case AST_FLOAT: return ast_r_scope(ast_copy_float(ast), ast->scope); break;
        case AST_LIST: return ast_r_scope(ast_copy_list(ast), ast->scope); break;
        case AST_BOOLEAN: return ast_r_scope(ast_copy_boolean(ast), ast->scope); break;
        case AST_INTEGER: return ast_r_scope(ast_copy_integer(ast), ast->scope); break;
        case AST_COMPOUND: return ast_r_scope(ast_copy_compound(ast), ast->scope); break;
        case AST_TYPE: return ast_r_scope(ast_copy_type(ast), ast->scope); break;
        case AST_ATTRIBUTE_ACCESS: return ast_r_scope(ast_copy_attribute_access(ast), ast->scope); break;
        case AST_LIST_ACCESS: return ast_r_scope(ast_copy_list_access(ast), ast->scope); break;
        case AST_BINOP: return ast_r_scope(ast_copy_binop(ast), ast->scope); break;
        case AST_NOOP: return ast; break;
        case AST_BREAK: return ast; break;
        case AST_RETURN: return ast_r_scope(ast_copy_return(ast), ast->scope); break;
        case AST_IF: return ast_r_scope(ast_copy_if(ast), ast->scope); break;
        case AST_WHILE: return ast_r_scope(ast_copy_while(ast), ast->scope); break;
        default: printf("WARNING\n"); return (void*)0; break;
    }
}
