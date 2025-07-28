#ifndef TOKEN_H
#define TOKEN_H

// token types for simpleLang
typedef enum
{
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_EQUAL,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_NEWLINE,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

// Token structure
typedef struct
{
    TokenType type;
    char *value;
    int line;
    int column;
} Token;

// function for creating token Which takes input TokenType and its value and returns pointer to Token structure
Token *create_token(TokenType type, char *value, int line, int column);

// function for converting created token to string which takes input tokentype and return char array
char *token_type_to_string(TokenType type);

// function for printing tokens
void print_token(Token *token);

// free the token after using it
void free_token(Token *token);

#endif