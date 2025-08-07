// define header guards
#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

#define ERROR_SIZE 256
// creating parser structure which parse the grammar of simplelang
typedef struct
{
    // Parser points to lexer
    Lexer *lexer;
    // Parser tracks current_token of simplelang
    Token *current_token;
    // Error flag
    int has_error;
    // Error message if encountered
    char error_message[ERROR_SIZE];
} Parser;

// Create Parser Function input->lexer and output->Parser
Parser *create_parser(Lexer *lexer);

// Deallocating dynamically allocated memory
void free_parser(Parser *parser);

// Parser functions for creating AST
ASTNode *parse_program(Parser *parser);
ASTNode *parse_statement(Parser *parser);
ASTNode *parse_declaration(Parser *parser);
ASTNode *parse_assignment(Parser *parser);
ASTNode *parse_if_statement(Parser *parser);
ASTNode *parse_expression(Parser *parser);
ASTNode *parse_block(Parser *parser);
ASTNode *parse_term(Parser *parser);
ASTNode *parse_factor(Parser *parser);
// Helper functions for parsing
void advance_token(Parser *parser);
void skip_newlines(Parser *parser);
int match_token(Parser *parser, TokenType expected);
int peek_token(Parser *parser, TokenType expected);
void parser_error(Parser *parser, char *message);

#endif
