#ifndef AST_H
#define AST_H
#include "token.h"
// create Nodetype which include grammar following
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
typedef enum
{
    AST_PROGRAM,
    AST_DECLARATION,
    AST_ASSIGNMENT,
    AST_IF_STATEMENT,
    AST_EXPRESSION,
    AST_BINARY_OP,
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_BLOCK
} ASTNodeType;

// Define the data structure of Abstract syntax tree(syntax tree)
// forward declaration
typedef struct ASTNode ASTNode;
typedef struct ASTNode
{
    ASTNodeType type;
    // depending on the production rule we have to create node for number,identifier... etc
    union
    {
        struct
        {
            int value;
        } number;
        struct
        {
            char *name;
        } identifier;
        struct
        {
            char *var_name;
            ASTNode *init_value;
        } declaration;
        struct
        {
            char *var_name;
            ASTNode *value;
        } assignment;
        struct
        {
            ASTNode *left;
            ASTNode *right;
            TokenType operator;
        } binary_op;
        struct
        {

            ASTNode *condition;
            ASTNode *then_block;
            ASTNode *else_block;
        } if_stmt;
        struct
        {
            ASTNode **statements;
            int count;
            int capacity;
        } block;
    } data;
    int line;   // tracks line number for error detection
    int column; // tracks column number for error detection
} ASTNode;

// let's define the construction of ast from parser which uses recursive decent parsing

ASTNode *create_program_node();
ASTNode *create_number_node(int value, int line, int column);
ASTNode *create_identifier_node(char *name, int line, int column);
ASTNode *create_binary_op_node(ASTNode *left, TokenType op, ASTNode *right, int line, int column);
ASTNode *create_declaration_node(char *var_name, ASTNode *init_value, int line, int column);
ASTNode *create_assignment_node(char *var_name, ASTNode *value, int line, int column);
ASTNode *create_if_node(ASTNode *condition, ASTNode *then_block, ASTNode *else_block, int line, int column);
ASTNode *create_block_node(void);

// block
void add_statement_to_block(ASTNode *block, ASTNode *statement);

void print_ast(ASTNode *node, int indent, int is_last, char *prefix);
void free_ast(ASTNode *node);

#endif