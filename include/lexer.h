#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct
{
    char *input;
    int position;
    int length;
    int line;
    int column;
    char current_char;
} Lexer;

Lexer *create_lexer(char *input);
void free_lexer(Lexer *lexer);
void advance(Lexer *lexer);
void skip_whitespace(Lexer *lexer);

Token *get_next_token(Lexer *lexer);
Token *read_number(Lexer *lexer);
Token *read_identifier(Lexer *lexer);
TokenType get_keyword_type(char *identifier);

int is_alpha(char c);

char *read_file(char *filename);
int tokenize_file(char *filename, char *output_filename);

#endif