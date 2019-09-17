#ifndef HERMES_BUILTINS_H
#define HERMES_BUILTINS_H
#include "AST.h"


AST_T* hermes_builtin_function_print(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_pprint(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_include(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_wad(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_lad(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_fopen(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_fclose(AST_T* self, dynamic_list_T* args);

AST_T* hermes_builtin_function_fputs(AST_T* self, dynamic_list_T* args);

AST_T* INITIALIZED_NOOP;
#endif
