#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/token.h"

Token *create_token(TokenType type, char *value, int line, int column)
{
    Token *token = malloc(sizeof(Token));
    if (!token)
        return NULL;
    token->type = type;
    token->line = line;
    token->column = column;
    if (value)
    {
        token->value = malloc(strlen(value) + 1);
        if (!token->value)
        {
            free(token);
            return NULL;
        }
        strcpy(token->value, value);
    }
    else
    {
        token->value = NULL;
    }
    return token;
}

char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_INT:
        return "INT";
    case TOKEN_IF:
        return "IF";
    case TOKEN_ELSE:
        return "ELSE";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_PLUS:
        return "PLUS";
    case TOKEN_MINUS:
        return "MINUS";
    case TOKEN_EQUAL:
        return "EQUAL";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_LBRACE:
        return "LBRACE";
    case TOKEN_RBRACE:
        return "RBRACE";
    case TOKEN_NEWLINE:
        return "NEWLINE";
    case TOKEN_EOF:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

void print_token(Token *token)
{
    if (!token)
    {
        printf("NULL token\n");
        return;
    }

    printf("%.12s %.10s Line :%3d Col:%2d\n",
           token_type_to_string(token->type),
           token->value ? token->value : "NULL",
           token->line,
           token->column);
}

void free_token(Token *token)
{
    if (token)
    {
        if (token->value)
        {
            free(token->value);
        }
        free(token);
    }
}
