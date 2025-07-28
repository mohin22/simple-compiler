#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"

Lexer *create_lexer(char *input)
{
    Lexer *lexer = malloc(sizeof(Lexer));
    if (!lexer)
        return NULL;

    lexer->length = strlen(input);
    lexer->input = malloc(lexer->length + 1);
    if (!lexer->input)
    {
        free(lexer);
        return NULL;
    }

    strcpy(lexer->input, input);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_char = lexer->length > 0 ? lexer->input[0] : '\0';

    return lexer;
}

void free_lexer(Lexer *lexer)
{
    if (lexer)
    {
        if (lexer->input)
            free(lexer->input);
        free(lexer);
    }
}

void advance(Lexer *lexer)
{
    if (lexer->position < lexer->length - 1)
    {
        if (lexer->current_char == '\n')
        {
            lexer->line++;
            lexer->column = 1;
        }
        else
        {
            lexer->column++;
        }
        lexer->position++;
        lexer->current_char = lexer->input[lexer->position];
    }
    else
    {
        lexer->current_char = '\0';
    }
}

void skip_whitespace(Lexer *lexer)
{
    while (lexer->current_char == ' ' || lexer->current_char == '\t')
    {
        advance(lexer);
    }
}

int is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int is_alnum(char c)
{
    return is_alpha(c) || is_digit(c);
}

Token *read_number(Lexer *lexer)
{
    int start_column = lexer->column;
    char buffer[64];
    int pos = 0;

    while (is_digit(lexer->current_char) && pos < 63)
    {
        buffer[pos++] = lexer->current_char;
        advance(lexer);
    }
    buffer[pos] = '\0';

    return create_token(TOKEN_NUMBER, buffer, lexer->line, start_column);
}

Token *read_identifier(Lexer *lexer)
{
    int start_column = lexer->column;
    char buffer[64];
    int pos = 0;

    while (is_alnum(lexer->current_char) && pos < 63)
    {
        buffer[pos++] = lexer->current_char;
        advance(lexer);
    }
    buffer[pos] = '\0';

    TokenType type = get_keyword_type(buffer);
    return create_token(type, buffer, lexer->line, start_column);
}

TokenType get_keyword_type(char *identifier)
{
    if (strcmp(identifier, "int") == 0)
        return TOKEN_INT;
    if (strcmp(identifier, "if") == 0)
        return TOKEN_IF;
    if (strcmp(identifier, "else") == 0)
        return TOKEN_ELSE;
    return TOKEN_IDENTIFIER;
}

Token *get_next_token(Lexer *lexer)
{
    while (lexer->current_char != '\0')
    {
        int line = lexer->line;
        int column = lexer->column;

        if (lexer->current_char == ' ' || lexer->current_char == '\t')
        {
            skip_whitespace(lexer);
            continue;
        }

        if (lexer->current_char == '\n')
        {
            advance(lexer);
            return create_token(TOKEN_NEWLINE, "\\n", line, column);
        }

        if (is_digit(lexer->current_char))
        {
            return read_number(lexer);
        }

        if (is_alpha(lexer->current_char))
        {
            return read_identifier(lexer);
        }

        switch (lexer->current_char)
        {
        case '=':
            if (lexer->position + 1 < lexer->length &&
                lexer->input[lexer->position + 1] == '=')
            {
                advance(lexer);
                advance(lexer);
                return create_token(TOKEN_EQUAL, "==", line, column);
            }
            advance(lexer);
            return create_token(TOKEN_ASSIGN, "=", line, column);

        case '+':
            advance(lexer);
            return create_token(TOKEN_PLUS, "+", line, column);

        case '-':
            advance(lexer);
            return create_token(TOKEN_MINUS, "-", line, column);

        case ';':
            advance(lexer);
            return create_token(TOKEN_SEMICOLON, ";", line, column);

        case '(':
            advance(lexer);
            return create_token(TOKEN_LPAREN, "(", line, column);

        case ')':
            advance(lexer);
            return create_token(TOKEN_RPAREN, ")", line, column);

        case '{':
            advance(lexer);
            return create_token(TOKEN_LBRACE, "{", line, column);

        case '}':
            advance(lexer);
            return create_token(TOKEN_RBRACE, "}", line, column);

        default:
            char error[2] = {lexer->current_char, '\0'};
            advance(lexer);
            return create_token(TOKEN_ERROR, error, line, column);
        }
    }

    return create_token(TOKEN_EOF, "EOF", lexer->line, lexer->column);
}

char *read_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Cannot open file '%s'\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

int tokenize_file(char *filename, char *output_filename)
{
    char *input = read_file(filename);
    if (!input)
        return 1;

    FILE *output = fopen(output_filename, "w");
    if (!output)
    {
        printf("Error: Cannot create output file '%s'\n", output_filename);
        free(input);
        return 1;
    }

    Lexer *lexer = create_lexer(input);
    if (!lexer)
    {
        free(input);
        fclose(output);
        return 1;
    }

    printf("Tokenizing: %s -> %s\n\n", filename, output_filename);

    Token *token;
    int count = 0;

    while ((token = get_next_token(lexer)) && token->type != TOKEN_EOF)
    {
        fprintf(output, "%-12s %-10s Line:%2d Col:%2d\n",
                token_type_to_string(token->type),
                token->value, token->line, token->column);

        print_token(token);
        count++;
        free_token(token);
    }

    if (token)
    {
        fprintf(output, "%-12s %-10s Line:%2d Col:%2d\n",
                token_type_to_string(token->type),
                token->value, token->line, token->column);
        print_token(token);
        free_token(token);
    }

    printf("\nTotal tokens: %d\n", count);

    free_lexer(lexer);
    free(input);
    fclose(output);
    return 0;
}