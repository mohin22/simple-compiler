#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/codegen.h"

CodeGenerator *create_codegen() {
    CodeGenerator *gen = malloc(sizeof(CodeGenerator));
    gen->instructions = malloc(sizeof(char*) * 100);
    gen->count = 0;
    gen->capacity = 100;
    return gen;
}

void emit_instruction(CodeGenerator *gen, const char *instruction) {
    if (gen->count >= gen->capacity) {
        gen->capacity *= 2;
        gen->instructions = realloc(gen->instructions, sizeof(char*) * gen->capacity);
    }
    gen->instructions[gen->count] = malloc(strlen(instruction) + 1);
    strcpy(gen->instructions[gen->count], instruction);
    gen->count++;
}

void generate_expression(CodeGenerator *gen, ASTNode *expr) {
    if (!expr) return;
    
    switch (expr->type) {
        case AST_NUMBER:
            {
                char instr[32];
                snprintf(instr, 32, "ldi A %d", expr->data.number.value);
                emit_instruction(gen, instr);
            }
            break;
            
        case AST_IDENTIFIER:
            {
                char instr[32];
                snprintf(instr, 32, "lda %%var_%s", expr->data.identifier.name);
                emit_instruction(gen, instr);
            }
            break;
            
        case AST_BINARY_OP:
            generate_expression(gen, expr->data.binary_op.left);
            emit_instruction(gen, "push A");
            generate_expression(gen, expr->data.binary_op.right);
            emit_instruction(gen, "mov B A");
            emit_instruction(gen, "pop A");
            
            switch (expr->data.binary_op.operator) {
                case TOKEN_PLUS:
                    emit_instruction(gen, "add");
                    break;
                case TOKEN_MINUS:
                    emit_instruction(gen, "sub");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void generate_statement(CodeGenerator *gen, ASTNode *stmt) {
    if (!stmt) return;
    
    switch (stmt->type) {
        case AST_DECLARATION:
            if (stmt->data.declaration.init_value) {
                generate_expression(gen, stmt->data.declaration.init_value);
                char instr[32];
                snprintf(instr, 32, "sta %%var_%s", stmt->data.declaration.var_name);
                emit_instruction(gen, instr);
            }
            break;
            
        case AST_ASSIGNMENT:
            generate_expression(gen, stmt->data.assignment.value);
            {
                char instr[32];
                snprintf(instr, 32, "sta %%var_%s", stmt->data.assignment.var_name);
                emit_instruction(gen, instr);
            }
            break;
            
        case AST_BLOCK:
            for (int i = 0; i < stmt->data.block.count; i++) {
                generate_statement(gen, stmt->data.block.statements[i]);
            }
            break;
        default:
            break;
    }
}

void generate_code(CodeGenerator *gen, ASTNode *ast) {
    emit_instruction(gen, ".text");
    emit_instruction(gen, "");
    
    if (ast->type == AST_PROGRAM) {
        for (int i = 0; i < ast->data.block.count; i++) {
            generate_statement(gen, ast->data.block.statements[i]);
        }
    }
    
    emit_instruction(gen, "hlt");
    emit_instruction(gen, "");
    emit_instruction(gen, ".data");
    emit_instruction(gen, "var_a = 0");
    emit_instruction(gen, "var_b = 0");
    emit_instruction(gen, "var_sum = 0");
    emit_instruction(gen, "var_diff = 0");
    emit_instruction(gen, "var_x = 0");
    emit_instruction(gen, "var_counter = 0");
    emit_instruction(gen, "var_result = 0");
}

void write_assembly_file(CodeGenerator *gen, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    
    for (int i = 0; i < gen->count; i++) {
        fprintf(file, "%s\n", gen->instructions[i]);
    }
    fclose(file);
}

void free_codegen(CodeGenerator *gen) {
    for (int i = 0; i < gen->count; i++) {
        free(gen->instructions[i]);
    }
    free(gen->instructions);
    free(gen);
}