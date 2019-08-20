#include "include/hermes_lexer.h"
#include "include/string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


char* hermes_char_to_string(char c)
{
    char* str = calloc(2, sizeof(char));
    str[0] = c;
    str[1] = '\0';

    return str;
}

hermes_lexer_T* init_hermes_lexer(char* contents)
{
    hermes_lexer_T* hermes_lexer = calloc(1, sizeof(struct HERMES_LEXER_STRUCT));
    hermes_lexer->contents = contents;
    hermes_lexer->char_index = 0;
    hermes_lexer->line_n = 1;
    hermes_lexer->current_char = hermes_lexer->contents[hermes_lexer->char_index];

    return hermes_lexer;
}

token_T* hermes_lexer_get_next_token(hermes_lexer_T* hermes_lexer)
{
    while (hermes_lexer->current_char != '\0' && hermes_lexer->char_index < strlen(hermes_lexer->contents) - 1)
    {
        while (hermes_lexer->current_char == ' ' || (int) hermes_lexer->current_char == 10 || (int) hermes_lexer->current_char == 13)
            hermes_lexer_skip_whitespace(hermes_lexer);

        if (isdigit(hermes_lexer->current_char))
            return hermes_lexer_collect_number(hermes_lexer);

        if (isalnum(hermes_lexer->current_char))
            return hermes_lexer_collect_id(hermes_lexer);

        if (hermes_lexer->current_char == '+')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);

            int type = TOKEN_PLUS;
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '=')
            {
                type = TOKEN_PLUS_EQUALS;
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
            }

            return init_token(type, value);
        }

        if (hermes_lexer->current_char == '-')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);

            int type = TOKEN_MINUS;
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '=')
            {
                type = TOKEN_MINUS_EQUALS;
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
            }

            return init_token(type, value);
        }

        if (hermes_lexer->current_char == '*')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);

            int type = TOKEN_STAR;
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '=')
            {
                type = TOKEN_STAR_EQUALS;
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
            }

            return init_token(type, value);
        }

        if (hermes_lexer->current_char == '&')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);
            
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '&')
            {
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
                
                return init_token(TOKEN_AND, value);
            }
        }

        if (hermes_lexer->current_char == '=')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);
            
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '=')
            {
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
                
                return init_token(TOKEN_EQUALS_EQUALS, value);
            }
            else
            {
                return init_token(TOKEN_EQUALS, value);
            }
        }

        if (hermes_lexer->current_char == '!')
        {
            char* value = hermes_char_to_string(hermes_lexer->current_char);
            
            hermes_lexer_advance(hermes_lexer);

            if (hermes_lexer->current_char == '=')
            {
                value = realloc(value, strlen(value) + 2);
                strcat(value, hermes_char_to_string(hermes_lexer->current_char));

                hermes_lexer_advance(hermes_lexer);
                
                return init_token(TOKEN_NOT_EQUALS, value);
            }

            return init_token(TOKEN_NOT, value);
        }

        switch(hermes_lexer->current_char)
        {
            case '"': return hermes_lexer_collect_string(hermes_lexer); break;
            case '\'': return hermes_lexer_collect_char(hermes_lexer); break;
            case '{': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_LBRACE); break;
            case '}': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_RBRACE); break;
            case '[': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_LBRACKET); break;
            case ']': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_RBRACKET); break;
            case '(': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_LPAREN); break;
            case ')': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_RPAREN); break;
            case ';': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_SEMI); break;
            case ',': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_COMMA); break;
            case '/': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_DIV); break;
            case '%': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_PERCENTAGE); break;
            case '.': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_DOT); break;
            case '<': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_LESS_THAN); break;
            case '>': return hermes_lexer_advance_with_token(hermes_lexer, TOKEN_LARGER_THAN); break;
            default: printf("[Line %d] Unexpected %c\n", hermes_lexer->line_n, hermes_lexer->current_char); exit(1); break;
        }
    }

    init_token(TOKEN_EOF, "\0");
}

token_T* hermes_lexer_advance_with_token(hermes_lexer_T* hermes_lexer, int type)
{
    char* value = hermes_char_to_string(hermes_lexer->current_char);
    hermes_lexer_advance(hermes_lexer);
    return init_token(type, value);
}

void hermes_lexer_advance(hermes_lexer_T* hermes_lexer)
{
    if (hermes_lexer->current_char == '\n' || hermes_lexer->current_char == 10)
        hermes_lexer->line_n += 1;

    if (hermes_lexer->current_char != '\0' && hermes_lexer->char_index < strlen(hermes_lexer->contents) - 1)
    {
        hermes_lexer->char_index += 1;
        hermes_lexer->current_char = hermes_lexer->contents[hermes_lexer->char_index];
    }
}

void hermes_lexer_expect_char(hermes_lexer_T* hermes_lexer, char c)
{
    if (hermes_lexer->current_char != c)
    {
        printf("Lexer expected the current char to be `%c`, but it was `%c`.", c, hermes_lexer->current_char);
        exit(1);
    }
}

void hermes_lexer_skip_whitespace(hermes_lexer_T* hermes_lexer)
{
    while (hermes_lexer->current_char == ' ' || (int) hermes_lexer->current_char == 10 || (int) hermes_lexer->current_char == 13)
        hermes_lexer_advance(hermes_lexer);
}

token_T* hermes_lexer_collect_string(hermes_lexer_T* hermes_lexer)
{
    hermes_lexer_expect_char(hermes_lexer, '"');
    hermes_lexer_advance(hermes_lexer);
    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (hermes_lexer->current_char != '"')
    {
        char* strchar = hermes_char_to_string(hermes_lexer->current_char);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        hermes_lexer_advance(hermes_lexer);
    }

    hermes_lexer_advance(hermes_lexer);

    return init_token(TOKEN_STRING_VALUE, buffer);
}

token_T* hermes_lexer_collect_char(hermes_lexer_T* hermes_lexer)
{
    hermes_lexer_expect_char(hermes_lexer, '\'');
    hermes_lexer_advance(hermes_lexer);
    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    int c = 0;

    while (hermes_lexer->current_char != '\'')
    {
        if (c > 1)
        {
            printf("Chars can only contain one character\n");
            exit(1);
        }

        char* strchar = hermes_char_to_string(hermes_lexer->current_char);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        hermes_lexer_advance(hermes_lexer);

        c++;
    }

    hermes_lexer_advance(hermes_lexer);

    return init_token(TOKEN_CHAR_VALUE, buffer);
}

token_T* hermes_lexer_collect_number(hermes_lexer_T* hermes_lexer)
{
    int type = TOKEN_INTEGER_VALUE;

    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (isdigit(hermes_lexer->current_char))
    {
        char* strchar = hermes_char_to_string(hermes_lexer->current_char);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        hermes_lexer_advance(hermes_lexer);
    }

    if (hermes_lexer->current_char == '.')
    {
        char* strchar = hermes_char_to_string(hermes_lexer->current_char);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        hermes_lexer_advance(hermes_lexer);

        type = TOKEN_FLOAT_VALUE;

        while (isdigit(hermes_lexer->current_char))
        {
            char* strchar = hermes_char_to_string(hermes_lexer->current_char);
            buffer = realloc(buffer, strlen(buffer) + 2);
            strcat(buffer, strchar);
            free(strchar);

            hermes_lexer_advance(hermes_lexer);
        }
    } 

    return init_token(type, buffer);
}

token_T* hermes_lexer_collect_id(hermes_lexer_T* hermes_lexer)
{
    char* buffer = calloc(1, sizeof(char));
    buffer[0] = '\0';

    while (isalnum(hermes_lexer->current_char) || hermes_lexer->current_char == '_')
    {
        char* strchar = hermes_char_to_string(hermes_lexer->current_char);
        buffer = realloc(buffer, strlen(buffer) + 2);
        strcat(buffer, strchar);
        free(strchar);

        hermes_lexer_advance(hermes_lexer);
    }

    return init_token(TOKEN_ID, buffer);
}

void hermes_lexer_dump(hermes_lexer_T* hermes_lexer)
{
    token_T* token;

    while (1)
    {
        token = hermes_lexer_get_next_token(hermes_lexer);

        printf("TOKEN(%d, %s)\n", token->type, token->value);

        if (token->type == TOKEN_EOF)
            break;
    }
}

void hermes_lexer_free(hermes_lexer_T* hermes_lexer)
{
    free(hermes_lexer->contents);
    free(hermes_lexer);
}
