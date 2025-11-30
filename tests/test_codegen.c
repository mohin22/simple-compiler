#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/codegen.h"

void test_simple_assignment() {
    printf("Testing simple assignment code generation...\n");
    
    char *input = "int a = 5;";
    Lexer *lexer = create_lexer(input);
    Parser *parser = create_parser(lexer);
    ASTNode *ast = parse_program(parser);
    
    assert(ast != NULL);
    assert(!parser->has_error);
    
    CodeGenerator *codegen = create_codegen();
    generate_code(codegen, ast);
    write_assembly_file(codegen, "output/test_assignment.asm");
    
    printf("Generated assembly for assignment test\n");
    
    free_codegen(codegen);
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
}

int main() {
    printf("=== Code Generator Tests ===\n\n");
    test_simple_assignment();
    printf("\nAll code generator tests passed!\n");
    return 0;
}