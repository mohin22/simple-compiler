#include "../include/parser.h"
#include "../include/ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Parser *create_parser(Lexer *lexer)
{
    // Create parser
    Parser *parser = malloc(sizeof(Parser));
    if (!parser)
    {
        return NULL;
    }
    // Assign the values of lexer to parser and update the has_error and error_message
    parser->lexer = lexer;
    parser->current_token = get_next_token(lexer);
    parser->has_error = 0;
    parser->error_message[0] = '\0';
    return parser;
}

void free_parser(Parser *parser)
{
    if (parser)
    {
        // if current_token created then free it also
        if (parser->current_token)
        {
            free_token(parser->current_token);
        }
        free(parser);
    }
}

/*Now the main logic of parser which takes lexer output and construct ABSTRACT SYNTAX TREE
  from Recursive Descent Parsing which is the top-down parsing technique
  It creates one function per production rule
  We will define each function for non-terminal which defines recursively or non-recursively terminal | nonterminal */
// Create AST

/*
program → statement*
statement → declaration | assignment | if_statement
declaration → 'int' IDENTIFIER ('=' expression)? ';'
assignment → IDENTIFIER '=' expression ';'
if_statement → 'if' '(' expression ')' block ('else' block)?
block → '{' statement* '}'
expression → term ('==' term)*
term → factor (('+' | '-') factor)*
factor → NUMBER | IDENTIFIER | '(' expression ')'
*/
// Clear the current token and move the pointer of parser token to next token
void advance_token(Parser *parser)
{
    if (parser->current_token)
    {
        free_token(parser->current_token);
    }
    parser->current_token = get_next_token(parser->lexer);
}
int match_token(Parser *parser, TokenType expected)
{
    if (parser->current_token && parser->current_token->type == expected)
    {
        advance_token(parser);
        return 1;
    }
    return 0;
}
int peek_token(Parser *parser, TokenType expected)
{
    return parser->current_token && parser->current_token->type == expected;
}
void skip_newlines(Parser *parser)
{
    while (peek_token(parser, TOKEN_NEWLINE))
    {
        advance_token(parser);
    }
}
void parser_error(Parser *parser, char *message)
{
    parser->has_error = 1;
    // print the error message with formatting
    snprintf(
        parser->error_message,
        sizeof(parser->error_message),
        "Parse error at line %d, column %d: %s",
        parser->current_token ? parser->current_token->line : 0,
        parser->current_token ? parser->current_token->column : 0,
        message);
}
ASTNode *parse_program(Parser *parser)
{
    ASTNode *program = create_program_node();
    // define function for skipping newlines if blank spaces
    skip_newlines(parser);
    // iteratively start parsing based on input lexer
    while (parser->current_token && parser->current_token->type != TOKEN_EOF && !parser->has_error)
    {
        // create statement node
        ASTNode *stmt = parse_statement(parser);
        if (stmt)
        {
            add_statement_to_block(program, stmt);
        }
        skip_newlines(parser);
    }
    // check for errors
    if (parser->has_error)
    {
        free_ast(program);
        return NULL;
    }
    return program;
}
ASTNode *parse_statement(Parser *parser)
{
    skip_newlines(parser);
    if (peek_token(parser, TOKEN_INT))
    {
        return parse_declaration(parser);
    }
    else if (peek_token(parser, TOKEN_IF))
    {
        return parse_if_statement(parser);
    }
    else if (peek_token(parser, TOKEN_IDENTIFIER))
    {
        return parse_assignment(parser);
    }
    else
    {
        parser_error(parser, "Expected statement");
        return NULL;
    }
}
ASTNode *parse_declaration(Parser *parser)
{
    if (!match_token(parser, TOKEN_INT))
    {
        parser_error(parser, "Expected int");
        return NULL;
    }
    if (!peek_token(parser, TOKEN_IDENTIFIER))
    {
        parser_error(parser, "Expected identifier after 'int'");
        return NULL;
    }
    char *var_name = malloc(strlen(parser->current_token->value) + 1);
    strcpy(var_name, parser->current_token->value);
    int line = parser->current_token->line;
    int column = parser->current_token->column;

    advance_token(parser);

    ASTNode *init_value = NULL;
    if (match_token(parser, TOKEN_ASSIGN))
    {
        init_value = parse_expression(parser);
        if (!init_value)
        {
            free(var_name);
            return NULL;
        }
    }
    if (!match_token(parser, TOKEN_SEMICOLON))
    {
        parser_error(parser, "Expected ';' after declaration");
        free(var_name);
        free_ast(init_value);
        return NULL;
    }
    ASTNode *decl = create_declaration_node(var_name, init_value, line, column);
    free(var_name);
    return decl;
}
ASTNode *parse_assignment(Parser *parser)
{
    if (!peek_token(parser, TOKEN_IDENTIFIER))
    {
        parser_error(parser, "Expected identifier");
        return NULL;
    }

    char *var_name = malloc(strlen(parser->current_token->value) + 1);
    strcpy(var_name, parser->current_token->value);
    int line = parser->current_token->line;
    int column = parser->current_token->column;
    advance_token(parser);
    if (!match_token(parser, TOKEN_ASSIGN))
    {
        parser_error(parser, "Expected '=' in assignment");
        free(var_name);
        return NULL;
    }
    ASTNode *value = parse_expression(parser);
    if (!value)
    {
        free(var_name);
        return NULL;
    }
    if (!match_token(parser, TOKEN_SEMICOLON))
    {
        parser_error(parser, "Expected ';' in assignment");
        free(var_name);
        free_ast(value);
        return NULL;
    }
    ASTNode *assign = create_assignment_node(var_name, value, line, column);
    free(var_name);
    return assign;
}
ASTNode *parse_if_statement(Parser *parser)
{
    int line = parser->current_token->line;
    int column = parser->current_token->column;

    if (!match_token(parser, TOKEN_IF))
    {
        parser_error(parser, "Expected 'if'");
        return NULL;
    }
    if (!match_token(parser, TOKEN_LPAREN))
    {
        parser_error(parser, "Expected '(' after 'if'");
        return NULL;
    }
    ASTNode *condition = parse_expression(parser);
    if (!condition)
    {
        return NULL;
    }
    if (!match_token(parser, TOKEN_RPAREN))
    {
        parser_error(parser, "Expected ')' after if condition");
        free_ast(condition);
        return NULL;
    }
    ASTNode *then_block = parse_block(parser);
    if (!then_block)
    {
        free_ast(condition);
        return NULL;
    }
    ASTNode *else_block = NULL;
    if (peek_token(parser, TOKEN_ELSE))
    {
        advance_token(parser);
        else_block = parse_block(parser);
        if (!else_block)
        {
            free_ast(condition);
            free_ast(then_block);
            return NULL;
        }
    }
    return create_if_node(condition, then_block, else_block, line, column);
}
ASTNode *parse_expression(Parser *parser)
{
    ASTNode *left = parse_term(parser);
    if (!left)
        return NULL;

    while (peek_token(parser, TOKEN_EQUAL))
    {
        TokenType op = parser->current_token->type;
        int line = parser->current_token->line;
        int column = parser->current_token->column;
        advance_token(parser);

        ASTNode *right = parse_term(parser);
        if (!right)
        {
            free_ast(left);
            return NULL;
        }
        left = create_binary_op_node(left, op, right, line, column);
    }
    return left;
}
ASTNode *parse_block(Parser *parser)
{
    if (!match_token(parser, TOKEN_LBRACE))
    {
        parser_error(parser, "Expected '{'");
        return NULL;
    }

    ASTNode *block = create_block_node();
    skip_newlines(parser);

    while (!peek_token(parser, TOKEN_RBRACE) && !peek_token(parser, TOKEN_EOF) && !parser->has_error)
    {
        ASTNode *stmt = parse_statement(parser);
        if (stmt)
        {
            add_statement_to_block(block, stmt);
        }
        skip_newlines(parser);
    }
    if (!match_token(parser, TOKEN_RBRACE))
    {
        parser_error(parser, "Expected '}'");
        free_ast(block);
        return NULL;
    }
    return block;
}
ASTNode *parse_term(Parser *parser)
{
    ASTNode *left = parse_factor(parser);
    if (!left)
        return NULL;

    while (peek_token(parser, TOKEN_PLUS) || peek_token(parser, TOKEN_MINUS))
    {
        TokenType op = parser->current_token->type;
        int line = parser->current_token->line;
        int column = parser->current_token->column;
        advance_token(parser);

        ASTNode *right = parse_factor(parser);
        if (!right)
        {
            free_ast(left);
            return NULL;
        }
        left = create_binary_op_node(left, op, right, line, column);
    }
    return left;
}
ASTNode *parse_factor(Parser *parser)
{
    if (peek_token(parser, TOKEN_NUMBER))
    {
        int value = atoi(parser->current_token->value);
        int line = parser->current_token->line;
        int column = parser->current_token->column;
        advance_token(parser);
        return create_number_node(value, line, column);
    }
    if (peek_token(parser, TOKEN_IDENTIFIER))
    {
        char *name = malloc(strlen(parser->current_token->value) + 1);
        strcpy(name, parser->current_token->value);
        int line = parser->current_token->line;
        int column = parser->current_token->column;
        advance_token(parser);

        ASTNode *node = create_identifier_node(name, line, column);
        free(name);
        return node;
    }

    if (match_token(parser, TOKEN_LPAREN))
    {
        ASTNode *expr = parse_expression(parser);
        if (!expr)
            return NULL;

        if (!match_token(parser, TOKEN_RPAREN))
        {
            parser_error(parser, "Expected ')' after expression");
            free_ast(expr);
            return NULL;
        }

        return expr;
    }

    parser_error(parser, "Expected number, identifier, or '('");
    return NULL;
}
