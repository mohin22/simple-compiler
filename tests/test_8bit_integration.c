#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/codegen.h"

void test_basic_arithmetic() {
    printf("Testing basic arithmetic code generation for 8-bit CPU...\n");
    
    char *input = "int a = 10; int b = 5; int sum = a + b;";
    Lexer *lexer = create_lexer(input);
    Parser *parser = create_parser(lexer);
    ASTNode *ast = parse_program(parser);
    
    assert(ast != NULL);
    assert(!parser->has_error);
    
    CodeGenerator *codegen = create_codegen();
    generate_code(codegen, ast);
    write_assembly_file(codegen, "output/test_add.asm");
    
    printf("Generated test_add.asm\n");
    
    free_codegen(codegen);
    free_ast(ast);
    free_parser(parser);
    free_lexer(lexer);
}

int main() {
    printf("=== 8-bit CPU Integration Tests ===\n\n");
    test_basic_arithmetic();
    printf("\nAll 8-bit CPU integration tests completed!\n");
    return 0;
}