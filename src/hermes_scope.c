#include "include/hermes_scope.h"


hermes_scope_T* init_hermes_scope()
{
    hermes_scope_T* hermes_scope = calloc(1, sizeof(struct HERMES_SCOPE_STRUCT));
    hermes_scope->variable_definitions = init_dynamic_list(sizeof(struct AST_STRUCT));
    hermes_scope->function_definitions = init_dynamic_list(sizeof(struct AST_STRUCT));

    return hermes_scope;
}

static void free_variable_definition(void* n)
{
    AST_T* node = (AST_T*) n;

    free(node->variable_name);

    if (node->variable_value != (void*) 0)
        free(node->variable_value);

    free(node);
}

static void free_function_definition(void* n)
{
    AST_T* node = (AST_T*) n;

    free(node->function_name);

    //TODO free function body
    
    free(node); // a lot of memory leaks right here.....
}

void hermes_scope_clear_variable_definitions(hermes_scope_T* scope)
{
    for (int i = scope->variable_definitions->size; i > 0; i--)
    {
        AST_T* node = (AST_T*) scope->variable_definitions->items[i];

        dynamic_list_remove(scope->variable_definitions, node, free_variable_definition);
    }    
}

void hermes_scope_clear_function_definitions(hermes_scope_T* scope)
{
    for (int i = scope->function_definitions->size; i > 0; i--)
    {
        AST_T* node = (AST_T*) scope->function_definitions->items[i];

        dynamic_list_remove(scope->function_definitions, node, free_function_definition);
    }
}
