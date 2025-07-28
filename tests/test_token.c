#include <stdio.h>
#include <stdlib.h>
#include "../include/token.h"

int main()
{
    Token *token[] = {
        create_token(TOKEN_INT, "int", 1, 1),
        create_token(TOKEN_IDENTIFIER, "a", 1, 5),
        create_token(TOKEN_ASSIGN, "=", 1, 7),
        create_token(TOKEN_NUMBER, "42", 1, 9),
        create_token(TOKEN_SEMICOLON, ";", 1, 11)};

    size_t len = sizeof(token) / sizeof(token[0]);
    for (int i = 0; i < len; i++)
    {
        print_token(token[i]);
        free(token[i]);
    }
    return 0;
}