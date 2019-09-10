#include "include/AST.h"
#include <stdlib.h>
#include <string.h>


AST_T* init_ast(int type)
{
    AST_T* AST = calloc(1, sizeof(struct AST_STRUCT));
    AST->type = type;
    AST->scope = (void*) 0;
    AST->return_value = (void*) 0;
    AST->fptr = (void*) 0;
    AST->object_children = (void*) 0;
    AST->saved_function_call = (void*) 0;
    AST->function_definitions = (void*) 0;
    AST->is_object_child = 0;

    if (type == AST_LIST)
        AST->function_definitions = init_dynamic_list(sizeof(struct AST_STRUCT*));

    return AST;
}
