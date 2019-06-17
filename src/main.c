#include "include/main.h"
#include "include/lexer.h"
#include "include/hermes_parser.h"
#include "include/hermes_runtime.h"
#include "include/io.h"


void print_help()
{
    printf(
        "Usage: \n"
        " hermes <input_file>\n"
    );
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_help();
        exit(1);
    }

    lexer_T* lexer = init_lexer(read_file(argv[1]));
    
    hermes_parser_T* parser = init_hermes_parser(lexer);
    AST_T* node = hermes_parser_parse(parser, (void*) 0);
    runtime_T* runtime = init_runtime();
    runtime_visit(runtime, node);

    lexer_free(lexer);
}
