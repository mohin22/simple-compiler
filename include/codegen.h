#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef struct {
    char **instructions;
    int count;
    int capacity;
} CodeGenerator;

CodeGenerator *create_codegen();
void generate_code(CodeGenerator *gen, ASTNode *ast);
void write_assembly_file(CodeGenerator *gen, const char *filename);
void free_codegen(CodeGenerator *gen);

#endif