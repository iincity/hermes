#ifndef HERMES_AST_H
#define HERMES_AST_H
#include <stdio.h>
#include "dynamic_list.h"

typedef struct AST_STRUCT
{
    enum {
        AST_OBJECT,
        AST_REFERENCE,
        AST_VARIABLE,
        AST_VARIABLE_DEFINITION,
        AST_VARIABLE_ASSIGNMENT,
        AST_VARIABLE_MODIFIER,
        AST_FUNCTION_DEFINITION,
        AST_FUNCTION_CALL,
        AST_NULL,
        AST_STRING,
        AST_CHAR,
        AST_FLOAT,
        AST_LIST,
        AST_BOOLEAN,
        AST_INTEGER,
        AST_COMPOUND,
        AST_TYPE,
        AST_BINOP,
        AST_NOOP,
        AST_BREAK,
        AST_RETURN,
        AST_IF,
        AST_ELSE,
        AST_WHILE,
        AST_ATTRIBUTE_ACCESS,
        AST_LIST_ACCESS
    } type;
    
    char* function_call_name;

    int int_value;
    unsigned int boolean_value;
    unsigned int is_object_child;
    float float_value;
    char char_value;
    char* string_value;
    char* type_value;
    char* variable_name;
    struct AST_STRUCT* variable_value;
    struct AST_STRUCT* variable_type;
    struct AST_STRUCT* variable_assignment_left;
    char* function_name;
    struct AST_STRUCT* binop_left;
    struct AST_STRUCT* binop_right;
    int binop_operator;
    dynamic_list_T* compound_value;
    dynamic_list_T* function_call_arguments;
    dynamic_list_T* function_definition_arguments;
    dynamic_list_T* object_children;
    dynamic_list_T* list_children;
    dynamic_list_T* function_definitions;
    struct AST_STRUCT* function_definition_body;
    struct AST_STRUCT* function_definition_type;
    struct AST_STRUCT* if_expr;
    struct AST_STRUCT* if_body;
    struct AST_STRUCT* if_otherwise;
    struct AST_STRUCT* else_expr;
    struct AST_STRUCT* while_expr;
    struct AST_STRUCT* while_body;
    struct AST_STRUCT* return_value;
    struct AST_STRUCT* list_access_pointer;
    struct AST_STRUCT* saved_function_call;
    
    struct hermes_scope_T* scope;

    struct AST_STRUCT* (*fptr)(struct AST_STRUCT* self, dynamic_list_T* args);
} AST_T;

AST_T* init_ast(int type);

void ast_set_key_value(AST_T* AST, char* key, void* value);

void* ast_get_value_by_key(AST_T* AST, char* key);

void ast_dump_keys(AST_T* AST);

typedef struct AST_ITEM_STRUCT
{
    char* key;
    void* value;
} AST_item_T;

AST_item_T* init_ast_item(char* key, void* value);

AST_T* ast_copy(AST_T* ast);
AST_T* ast_copy_object(AST_T* ast);
AST_T* ast_copy_variable(AST_T* ast);
AST_T* ast_copy_variable_definition(AST_T* ast);
AST_T* ast_copy_function_definition(AST_T* ast);
AST_T* ast_copy_string(AST_T* ast);
AST_T* ast_copy_char(AST_T* ast);
AST_T* ast_copy_float(AST_T* ast);
AST_T* ast_copy_list(AST_T* ast);
AST_T* ast_copy_boolean(AST_T* ast);
AST_T* ast_copy_integer(AST_T* ast);
AST_T* ast_copy_compound(AST_T* ast);
AST_T* ast_copy_type(AST_T* ast);
AST_T* ast_copy_attribute_access(AST_T* ast);
AST_T* ast_copy_return(AST_T* ast);
AST_T* ast_copy_variable_assignment(AST_T* ast);
AST_T* ast_copy_variable_modifier(AST_T* ast);
AST_T* ast_copy_function_call(AST_T* ast);
AST_T* ast_copy_null(AST_T* ast);
AST_T* ast_copy_list_access(AST_T* ast);
AST_T* ast_copy_binop(AST_T* ast);
AST_T* ast_copy_if(AST_T* ast);
AST_T* ast_copy_while(AST_T* ast);
#endif
