#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/ast.h"
// create the starting node
ASTNode *create_program_node()
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
    {
        return NULL;
    }

    node->type = AST_PROGRAM;
    node->data.block.statements = malloc(sizeof(ASTNode *) * 10); // create some pre - memory for statements
    node->data.block.count = 0;
    node->data.block.capacity = 10;
    node->line = 1;
    node->column = 1;
    return node;
}
// Create AST Node if the parser encounters number literal
ASTNode *create_number_node(int value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_NUMBER;
    node->data.number.value = value;
    node->line = line;
    node->column = column;

    return node;
}
// Create AST Node for Identifier
ASTNode *create_identifier_node(char *name, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_IDENTIFIER;
    node->data.identifier.name = malloc(strlen(name) + 1);
    if (!node->data.identifier.name)
    {
        free(node);
        return NULL;
    }
    strcpy(node->data.identifier.name, name);
    node->line = line;
    node->column = column;

    return node;
}
// Create AST subtree node for expression parsing with children nodes based on precedence and associtivity rule
ASTNode *create_binary_op_node(ASTNode *left, TokenType op, ASTNode *right, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_BINARY_OP;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    node->data.binary_op.operator = op;
    node->line = line;
    node->column = column;

    return node;
}
// Create the declaration node with value initialisation
ASTNode *create_declaration_node(char *var_name, ASTNode *init_value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_DECLARATION;
    node->data.declaration.var_name = malloc(strlen(var_name) + 1);
    if (!node->data.declaration.var_name)
    {
        free(node);
        return NULL;
    }
    strcpy(node->data.declaration.var_name, var_name);
    node->data.declaration.init_value = init_value;
    node->line = line;
    node->column = column;

    return node;
}
// Create AST assignment node
ASTNode *create_assignment_node(char *var_name, ASTNode *value, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_ASSIGNMENT;
    node->data.assignment.var_name = malloc(strlen(var_name) + 1);
    if (!node->data.assignment.var_name)
    {
        free(node);
        return NULL;
    }
    strcpy(node->data.assignment.var_name, var_name);
    node->data.assignment.value = value;
    node->line = line;
    node->column = column;

    return node;
}
// Create AST if node
ASTNode *create_if_node(ASTNode *condition, ASTNode *then_block, ASTNode *else_block, int line, int column)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_IF_STATEMENT;
    node->data.if_stmt.condition = condition;
    node->data.if_stmt.then_block = then_block;
    node->data.if_stmt.else_block = else_block;
    node->line = line;
    node->column = column;

    return node;
}
// create AST node for block
ASTNode *create_block_node()
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node)
        return NULL;

    node->type = AST_BLOCK;
    node->data.block.statements = malloc(sizeof(ASTNode *) * 10);
    node->data.block.count = 0;
    node->data.block.capacity = 10;
    node->line = 1;
    node->column = 1;

    return node;
}
// create AST node for adding statement to block node
void add_statement_to_block(ASTNode *block, ASTNode *statement)
{
    if (!block || (block->type != AST_BLOCK && block->type != AST_PROGRAM))
        return;
    if (!statement)
        return;

    // Resize if needed
    if (block->data.block.count >= block->data.block.capacity)
    {
        block->data.block.capacity *= 2;
        block->data.block.statements = realloc(block->data.block.statements,
                                               sizeof(ASTNode *) * block->data.block.capacity);
    }

    block->data.block.statements[block->data.block.count++] = statement;
}
// Print the AST based on indentation
void print_ast(ASTNode *node, int indent, int is_last, char *prefix)
{
    if (!node)
        return;

    // Print the current prefix
    if (prefix)
        printf("%s", prefix);

    // Print the tree connector
    if (indent > 0)
    {
        if (is_last)
            printf("└── ");
        else
            printf("├── ");
    }

    switch (node->type)
    {
    case AST_PROGRAM:
        printf("PROGRAM\n");
        for (int i = 0; i < node->data.block.count; i++)
        {
            char new_prefix[256] = {0};
            if (prefix)
                strcpy(new_prefix, prefix);
            print_ast(node->data.block.statements[i], indent + 1,
                      i == node->data.block.count - 1, new_prefix);
        }
        break;

    case AST_DECLARATION:
        printf("DECLARATION: %s\n", node->data.declaration.var_name);
        if (node->data.declaration.init_value)
        {
            char new_prefix[256] = {0};
            if (prefix)
                strcpy(new_prefix, prefix);
            if (is_last)
                strcat(new_prefix, "    ");
            else
                strcat(new_prefix, "│   ");
            print_ast(node->data.declaration.init_value, indent + 1, 1, new_prefix);
        }
        break;

    case AST_ASSIGNMENT:
        printf("ASSIGNMENT: %s\n", node->data.assignment.var_name);
        char assign_prefix[256] = {0};
        if (prefix)
            strcpy(assign_prefix, prefix);
        if (is_last)
            strcat(assign_prefix, "    ");
        else
            strcat(assign_prefix, "│   ");
        print_ast(node->data.assignment.value, indent + 1, 1, assign_prefix);
        break;

    case AST_IF_STATEMENT:
        printf("IF\n");
        char if_prefix[256] = {0};
        if (prefix)
            strcpy(if_prefix, prefix);
        if (is_last)
            strcat(if_prefix, "    ");
        else
            strcat(if_prefix, "│   ");

        // Print condition
        printf("%s├── CONDITION:\n", if_prefix);
        char cond_prefix[256] = {0};
        strcpy(cond_prefix, if_prefix);
        strcat(cond_prefix, "│   ");
        print_ast(node->data.if_stmt.condition, indent + 2, 1, cond_prefix);

        // Print then block
        int has_else = node->data.if_stmt.else_block != NULL;
        printf("%s%s THEN:\n", if_prefix, has_else ? "├──" : "└──");
        char then_prefix[256] = {0};
        strcpy(then_prefix, if_prefix);
        strcat(then_prefix, has_else ? "│   " : "    ");
        print_ast(node->data.if_stmt.then_block, indent + 2, 1, then_prefix);

        // Print else block if exists
        if (node->data.if_stmt.else_block)
        {
            printf("%s└── ELSE:\n", if_prefix);
            char else_prefix[256] = {0};
            strcpy(else_prefix, if_prefix);
            strcat(else_prefix, "    ");
            print_ast(node->data.if_stmt.else_block, indent + 2, 1, else_prefix);
        }
        break;

    case AST_BINARY_OP:
        printf("BINARY_OP: %s\n", token_type_to_string(node->data.binary_op.operator));
        char bin_prefix[256] = {0};
        if (prefix)
            strcpy(bin_prefix, prefix);
        if (is_last)
            strcat(bin_prefix, "    ");
        else
            strcat(bin_prefix, "│   ");
        print_ast(node->data.binary_op.left, indent + 1, 0, bin_prefix);
        print_ast(node->data.binary_op.right, indent + 1, 1, bin_prefix);
        break;

    case AST_NUMBER:
        printf("NUMBER: %d\n", node->data.number.value);
        break;

    case AST_IDENTIFIER:
        printf("IDENTIFIER: %s\n", node->data.identifier.name);
        break;

    case AST_BLOCK:
        printf("BLOCK\n");
        char block_prefix[256] = {0};
        if (prefix)
            strcpy(block_prefix, prefix);
        if (is_last)
            strcat(block_prefix, "    ");
        else
            strcat(block_prefix, "│   ");
        for (int i = 0; i < node->data.block.count; i++)
        {
            print_ast(node->data.block.statements[i], indent + 1,
                      i == node->data.block.count - 1, block_prefix);
        }
        break;

    default:
        printf("UNKNOWN NODE TYPE\n");
        break;
    }
}
// clean the dynamic allocated memory
void free_ast(ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_PROGRAM:
    case AST_BLOCK:
        for (int i = 0; i < node->data.block.count; i++)
        {
            free_ast(node->data.block.statements[i]);
        }
        free(node->data.block.statements);
        break;

    case AST_DECLARATION:
        free(node->data.declaration.var_name);
        free_ast(node->data.declaration.init_value);
        break;

    case AST_ASSIGNMENT:
        free(node->data.assignment.var_name);
        free_ast(node->data.assignment.value);
        break;

    case AST_IF_STATEMENT:
        free_ast(node->data.if_stmt.condition);
        free_ast(node->data.if_stmt.then_block);
        free_ast(node->data.if_stmt.else_block);
        break;

    case AST_BINARY_OP:
        free_ast(node->data.binary_op.left);
        free_ast(node->data.binary_op.right);
        break;

    case AST_IDENTIFIER:
        free(node->data.identifier.name);
        break;

    case AST_NUMBER:
        break;

    default:
        break;
    }

    free(node);
}
