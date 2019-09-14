#ifndef HERMES_BUILTINS_H
#define HERMES_BUILTINS_H
#include "AST.h"


AST_T* hermes_builtin_function_print(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_pprint(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_include(AST_T* self, dynamic_list_T* args);

AST_T* INITIALIZED_NOOP;
#endif
