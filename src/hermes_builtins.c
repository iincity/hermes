#include "include/hermes_builtins.h"
#include "include/hermes_runtime.h"
#include "include/hermes_lexer.h"
#include "include/hermes_parser.h"
#include "include/io.h"
#include <string.h>

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

/**
 * Builtin method to dump an AST node to disk, serialized.
 *
 * @param AST_T* self
 * @param dynamic_list_T* args
 *
 * @return AST_T*
 */
AST_T* hermes_builtin_function_wad(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 2, (int[]) {AST_COMPOUND, AST_STRING});

    AST_T* ast_compound = (AST_T*) args->items[0];
    AST_T* ast_string = (AST_T*) args->items[1];
    
    char* filename = ast_string->string_value;

    FILE *outfile;

    const char* filename_template = "%s.dat";
    char* fname = calloc(strlen(filename) + strlen(filename_template) + 1, sizeof(char));
    sprintf(fname, filename_template, filename);
      
    outfile = fopen(fname, "w");

    if (outfile == NULL) 
    { 
        fprintf(stderr, "Could not open %s\n", fname);
        free(fname); 
        exit(1); 
    } 
  
    // write struct to file 
    fwrite (&*ast_compound, sizeof(struct AST_STRUCT), 1, outfile);
      
    if(fwrite != 0)
    {
        // silence
    }
    else
    {
        printf("Could not write to %s\n", fname);
    }
    
    free(fname); 
  
    fclose(outfile); 
  
    return ast_compound;
}

/**
 * Built-in function to load a compound AST from disk
 *
 * @param AST_T* self
 * @param dynamic_list_T* args
 *
 * @return AST_T*
 */
AST_T* hermes_builtin_function_lad(AST_T* self, dynamic_list_T* args)
{
    runtime_expect_args(args, 1, (int[]) {AST_STRING});
    
    AST_T* ast_string = (AST_T*) args->items[0];

    char* filename = ast_string->string_value;

    const char* filename_template = "%s.dat";
    char* fname = calloc(strlen(filename) + strlen(filename_template) + 1, sizeof(char));
    sprintf(fname, filename_template, filename);

    FILE *infile; 
    struct AST_STRUCT input; 
      
    infile = fopen(fname, "r"); 
    if (infile == NULL) 
    { 
        fprintf(stderr, "Error reading %s\n", fname); 
        free(fname);
        exit(1); 
    } 
      
    // read file contents till end of file 
    while(fread(&input, sizeof(struct AST_STRUCT), 1, infile)) 
        printf("...\n");
  
    // close file 
    fclose (infile);

    AST_T* loaded = &input;

    return loaded;
}
