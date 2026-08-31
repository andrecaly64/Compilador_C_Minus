#ifndef CGEN_H
#define CGEN_H
#include "syntax_tree.h"

typedef enum {
    OP_ADD, OP_SUB, OP_MULT, OP_DIV,
    OP_ASSIGN,
    OP_EQ, OP_LT, OP_GT, OP_LE, OP_NEQ, OP_GE,
    OP_GOTO, OP_IF_FALSE, OP_LABEL,
    OP_CALL, OP_PARAM,
    OP_RETURN, 
    OP_VEC_READ, 
    OP_VEC_WRITE,
    OP_HALT,
    OP_FORMAL_PARAM
} opkind;

typedef struct quadrupla {
    opkind op;
    char* arg1;
    char* arg2;
    char* result;
    struct quadrupla* prox;
} quadrupla;

void gerador_codigo(treeNode* root);
char* gerador_expressao(treeNode* node);
void emit(opkind op, char* arg1, char* arg2, char* result);
void print_quadruplas();
void gerador_assembly();

#endif