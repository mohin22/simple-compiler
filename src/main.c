#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"

int main(int argc, char *argv[])
{
    // take command line argument for input file from input.sl(simple lang)
    if (argc != 2)
    {
        printf("FILE IS MISSING");
        return 1;
    }
    // store in character buffer processing
    char *input_filename = argv[1];
    char *input_content = read_file(input_filename);
    if (!input_content)
    {
        printf("Error: Failed to read input file '%s'\n", input_filename);
        return 1;
    }
    // Create lexer and feed the input.sl file for input of lexer
    printf("Lexical Analysis (Tokenization)\n\n");
    Lexer *lexer = create_lexer(input_content);
    if (!lexer)
    {
        printf("Error: Failed to create lexer\n");
        free(input_content);
        return 1;
    }
    // temporary lexer for printing the lexer
    Lexer *temp_lexer = create_lexer(input_content);
    Token *token;
    int token_count = 0;
    // loop through all input file for printing lexer output
    while ((token = get_next_token(temp_lexer)) && token->type != TOKEN_EOF)
    {
        print_token(token);
        token_count++;
        free_token(token);
    }

    if (token)
    {
        print_token(token);
        free_token(token);
    }
    // Count the number of tokens generated
    printf("Total tokens: %d\n\n", token_count);

    free_lexer(temp_lexer);
    // Give output of lexer to the parser
    printf("Syntax Analysis (Parsing)\n");
    Parser *parser = create_parser(lexer);
    if (!parser)
    {
        printf("Error: Failed to create parser\n");
        free_lexer(lexer);
        free(input_content);
        return 1;
    }
    // Generate ABSTRACT SYNTAX TREE from parser
    ASTNode *ast = parse_program(parser);
    // If parser encountered error print parser error
    if (parser->has_error)
    {
        printf("PARSING FAILED!\n");
        printf("Error: %s\n", parser->error_message);
        free_parser(parser);
        free_lexer(lexer);
        free(input_content);
        return 1;
    }
    else if (ast)
    {

        printf("PARSING SUCCESSFUL!\n");
        printf("Generated Abstract Syntax Tree (AST):\n\n");
        print_ast(ast, 0, 1, "");
        free_ast(ast);
    }
    // Throw error if AST not generated
    else
    {
        printf("PARSING FAILED!\n");
        printf("Error: No AST generated (unknown error)\n");
        free_parser(parser);
        free_lexer(lexer);
        free(input_content);
        return 1;
    }
    // Clean the dynamic allocated memory
    free_parser(parser);
    free_lexer(lexer);
    free(input_content);

    return 0;
}