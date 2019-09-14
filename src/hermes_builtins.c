#include "include/hermes_builtins.h"
#include "include/hermes_runtime.h"
#include "include/hermes_lexer.h"
#include "include/hermes_parser.h"
#include "include/io.h"

/**
 * Simple print implementation.
 *
 * @param AST_T* self
 * @param dynamic_list_T* args
 *
 * @return AST_T*
 */
AST_T* hermes_builtin_function_print(AST_T* self, dynamic_list_T* args)
{
    for (int i = 0; i < args->size; i++)
    {
        AST_T* ast_arg = (AST_T*) args->items[i];

        switch (ast_arg->type)
        {
            case AST_NULL: printf("NULL\n"); break;
            case AST_STRING: printf("%s\n", ast_arg->string_value); break;
            case AST_CHAR: printf("%c\n", ast_arg->char_value); break;
            case AST_INTEGER: printf("%d\n", ast_arg->int_value); break;
            case AST_FLOAT: printf("%0.6f\n", ast_arg->float_value); break;
            case AST_BOOLEAN: printf("%d\n", ast_arg->boolean_value); break;
            case AST_OBJECT: printf("{ object }\n"); break;
            case AST_LIST: printf("[ list ]\n"); break;
            case AST_COMPOUND: printf("<source/>\n"); break;
        }
    }

    return INITIALIZED_NOOP;
}

/**
 * Print the adress of a value
 *
 * @param AST_T* self
 * @param dynamic_list_T* args
 *
 * @return AST_T*
 */
AST_T* hermes_builtin_function_pprint(AST_T* self, dynamic_list_T* args)
{
    for (int i = 0; i < args->size; i++)
        printf("%p\n", (AST_T*) args->items[i]);

    return INITIALIZED_NOOP;
}

/**
 * Method for including other scripts, this will return the root node
 * as a compound.
 *
 * @param AST_T* self
 * @param dynamic_list_T* args
 *
 * @return AST_T*
 */
AST_T* hermes_builtin_function_include(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]) {AST_STRING});

    AST_T* ast_str = (AST_T*) args->items[0];
    char* filename = ast_str->string_value;

    hermes_lexer_T* lexer = init_hermes_lexer(read_file(filename));
    hermes_parser_T* parser = init_hermes_parser(lexer);
    AST_T* node = hermes_parser_parse(parser, (void*) 0);

    hermes_lexer_free(lexer);

    return node;
}
