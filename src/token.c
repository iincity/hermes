#include "include/token.h"
#include <stdlib.h>
#include <string.h>


token_T* init_token(int type, char* value)
{
    token_T* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = calloc(strlen(value) + 1, sizeof(char));
    strcpy(token->value, value);

    return token;
}

void token_free(token_T* token)
{
    free(token->value);
    free(token);
}
