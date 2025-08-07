#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
// tester function based on input with description
void test_parser(char *input, char *description)
{
    printf("\n=== Testing: %s ===\n", description);
    printf("Input: %s\n", input);

    Lexer *lexer = create_lexer(input);
    if (!lexer)
    {
        printf("Failed to create lexer\n");
        return;
    }

    Parser *parser = create_parser(lexer);
    if (!parser)
    {
        printf("Failed to create parser\n");
        free_lexer(lexer);
        return;
    }

    ASTNode *ast = parse_program(parser);

    if (parser->has_error)
    {
        printf("Parser Error: %s\n", parser->error_message);
    }
    else if (ast)
    {
        printf("AST:\n");
        print_ast(ast, 0, 1, "");
        free_ast(ast);
    }
    else
    {
        printf("No AST generated\n");
    }

    free_parser(parser);
    free_lexer(lexer);
}

int main()
{

    // Test 1: Simple declaration
    test_parser("int a;", "Simple declaration");

    // Test 2: Declaration with initialization
    test_parser("int a = 42;", "Declaration with initialization");

    // Test 3: Assignment
    test_parser("a = 10;", "Simple assignment");

    // Test 4: Expression with addition
    test_parser("int result = a + b;", "Expression with addition");

    // Test 5: Multiple statements
    test_parser("int a = 5;\nint b = 10;\na = a + b;", "Multiple statements");

    // Test 6: If statement
    test_parser("if (a == 5) {\n  b = 10;\n}", "If statement");

    // Test 7: If-else statement
    test_parser("if (a == 5) {\n  b = 10;\n} else {\n  b = 20;\n}", "If-else statement");

    // Test 8: Complex expression
    test_parser("int result = (a + b) - c;", "Complex expression with parentheses");

    return 0;
}