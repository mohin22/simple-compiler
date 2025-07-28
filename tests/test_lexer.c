#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"
#include "token.h"
int main()
{

    char *test_input = "int a = 42;";

    Lexer *lexer = create_lexer(test_input);
    Token *token;
    while ((token = get_next_token(lexer)) && token->type != TOKEN_EOF)
    {
        print_token(token);
        free_token(token);
    }

    if (token)
    {
        print_token(token);
        free_token(token);
    }

    free_lexer(lexer);
    return 0;
}