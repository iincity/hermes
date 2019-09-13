#include "include/main.h"
#include "include/test_hermes_lexer.h"
#include "../../src/include/hermes_lexer.h"


void test_hermes_lexer_behavior(void** state)
{
    hermes_lexer_T* hermes_lexer = init_hermes_lexer(
        "{ actor = { width = 32; height = 32; texture = \"/tmp\"; }; }"
    );

    assert_true(hermes_lexer != NULL);
    
    assert_true(hermes_lexer->current_char == '{');

    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_LBRACE);

    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_ID);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_EQUALS);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_LBRACE);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_ID);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_EQUALS);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_INTEGER_VALUE);

    assert_true(hermes_lexer->current_char == ';');

    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_SEMI);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_ID);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_EQUALS);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_INTEGER_VALUE);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_SEMI);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_ID);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_EQUALS);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_STRING_VALUE);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_SEMI);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_RBRACE);
    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_SEMI);

    assert_true(hermes_lexer_get_next_token(hermes_lexer)->type == TOKEN_RBRACE);
    assert_true(hermes_lexer->current_char == '\0');
}
