#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgen.h"
#include "node.h"

static int tempcount = 0;
static int labelcount = 0;
quadrupla* inicio = NULL;
quadrupla* fim = NULL;

static char* newTemp() {
    char* temp = malloc(10 * sizeof(char));
    sprintf(temp, "_t%d", tempcount++);
    return temp;
}

static char* newLabel() {
    char* label = malloc(10 * sizeof(char));
    sprintf(label, "L%d", labelcount++);
    return label;
}

//guardar quadruplas na lista
void emit(opkind op, char* arg1, char* arg2, char* result) {
    quadrupla* q = (quadrupla*) malloc(sizeof(quadrupla));
    q->op = op;
    q->arg1 = arg1 ? strdup(arg1) : NULL;
    q->arg2 = arg2 ? strdup(arg2) : NULL;
    q->result = result ? strdup(result) : NULL;
    q->prox = NULL;

    if (inicio == NULL) {
        inicio = q;
        fim = q;
    } else {
        fim->prox = q;
        fim = q;
    }
}

char* gerador_expressao(treeNode* node) {
    if (node == NULL) return NULL;

    //chamada de funcao
    if (node->node == stmt && node->nodeSubType.stmt == stmtFunc) {
        treeNode* arg = node->child[1]; 
        
        while (arg != NULL) {
            char* arg_val = gerador_expressao(arg);
            emit(OP_PARAM, arg_val, NULL, NULL);
            arg = arg->sibling;
        }
        
        char* result = newTemp();
        emit(OP_CALL, node->key.name, NULL, result);
        return result;
    }

    if (node->node != exp) return NULL;

    if (node->nodeSubType.exp == expNum) {
        char* numStr = malloc(16 * sizeof(char));
        sprintf(numStr, "%d", node->key.value);
        return numStr;
    }
    //leitura de valores do vetor
    else if (node->nodeSubType.exp == expId) {
        if (node->child[0] != NULL) {
            char* index = gerador_expressao(node->child[0]); 
            char* result = newTemp(); 
            
            emit(OP_VEC_READ, node->key.name, index, result); 
            return result;
        } else {
            return strdup(node->key.name); 
        }
    }
    else if (node->nodeSubType.exp == expOp) {
        char* left_arg = gerador_expressao(node->child[0]);
        char* right_arg = gerador_expressao(node->child[1]);
        char* result = newTemp();

        opkind op;
        if (node->key.op == 14) op = OP_ADD;
        else if (node->key.op == 15) op = OP_SUB;
        else if (node->key.op == 16) op = OP_MULT;
        else if (node->key.op == 17) op = OP_DIV;

        else if (node->key.op == 10) op = OP_LT;
        else if (node->key.op == 11) op = OP_LE;
        else if (node->key.op == 12) op = OP_GT;
        else if (node->key.op == 13) op = OP_GE;
        else if (node->key.op == 8) op = OP_EQ;
        else if (node->key.op == 9) op = OP_NEQ;

        emit(op, left_arg, right_arg, result);
        return result;
    }
    
    return NULL;
}

void gerador_codigo(treeNode* root) {
    if (root == NULL) return;

    treeNode* atual = root;

    while (atual != NULL) {
        //Declaração de Função
        if (atual->node == decl && atual->nodeSubType.decl == declFunc) {
            emit(OP_LABEL, atual->key.name, NULL, NULL);

            treeNode* param = atual->child[0];

            while (param != NULL) {
                char* nome_param = NULL;
                if (param->node == decl && param->nodeSubType.decl == declIdType && param->child[0] != NULL) {
                    nome_param = param->child[0]->key.name;
                } else {
                    nome_param = param->key.name;
                }

                if (nome_param != NULL && strcmp(nome_param, "void") != 0) {
                    emit(OP_FORMAL_PARAM, nome_param, NULL, NULL);
                }
                param = param->sibling;
            }
            
            gerador_codigo(atual->child[1]);
            
            if (strcmp(atual->key.name, "main") == 0) {
                emit(OP_HALT, NULL, NULL, NULL); 
            } else {
                if (fim != NULL && fim->op != OP_RETURN) {
                    emit(OP_RETURN, NULL, NULL, NULL);
                }
            }
        }
        // Atribuição e atribuição de vetor
        else if (atual->node == stmt && atual->nodeSubType.stmt == stmtAttrib) {
            char* right_val = gerador_expressao(atual->child[1]);
            treeNode* var_node = atual->child[0];
            
            if (var_node->child[0] != NULL) {
                char* index = gerador_expressao(var_node->child[0]);
                emit(OP_VEC_WRITE, index, right_val, var_node->key.name);
            } else {
                emit(OP_ASSIGN, right_val, NULL, var_node->key.name);
            }
        }
        //if
        else if (atual->node == stmt && atual->nodeSubType.stmt == stmtIf) {
            char* condition_result = gerador_expressao(atual->child[0]);
            char* label_else = newLabel();
            char* label_fim = newLabel();
            
            emit(OP_IF_FALSE, condition_result, NULL, label_else);
            gerador_codigo(atual->child[1]);
            emit(OP_GOTO, NULL, NULL, label_fim);
            emit(OP_LABEL, label_else, NULL, NULL);
            
            if (atual->child[2] != NULL) {
                gerador_codigo(atual->child[2]);
            }
            emit(OP_LABEL, label_fim, NULL, NULL);
        }
        //while
        else if (atual->node == stmt && atual->nodeSubType.stmt == stmtWhile) {
            char* label_start = newLabel();
            char* label_end = newLabel();
            
            emit(OP_LABEL, label_start, NULL, NULL);
            char* condition_result = gerador_expressao(atual->child[0]);
            emit(OP_IF_FALSE, condition_result, NULL, label_end);
            gerador_codigo(atual->child[1]);
            emit(OP_GOTO, NULL, NULL, label_start);
            emit(OP_LABEL, label_end, NULL, NULL);
        }
        // Retorno (Return)
        else if (atual->node == stmt && atual->nodeSubType.stmt == stmtReturn) {
            if (atual->child[0] != NULL) {
                char* ret_val = gerador_expressao(atual->child[0]);
                emit(OP_RETURN, ret_val, NULL, NULL);
            } else {
                emit(OP_RETURN, NULL, NULL, NULL);
            }
        }
        //chamada de função
        else if (atual->node == stmt && atual->nodeSubType.stmt == stmtFunc) {
            treeNode* arg = atual->child[1]; 
            
            while (arg != NULL) {
                char* arg_val = gerador_expressao(arg);
                emit(OP_PARAM, arg_val, NULL, NULL);
                arg = arg->sibling;
            }
            emit(OP_CALL, atual->key.name, NULL, NULL);
        }
        else {
            int i;
            for (i = 0; i < CHILD_MAX_NODES; i++) {
                if (atual->child[i] != NULL) {
                    gerador_codigo(atual->child[i]);
                }
            }
        }
        atual = atual->sibling;
    }
}

void print_quadruplas() {
    quadrupla* atual = inicio;
    printf("\n");

    while (atual != NULL) {
        char* op_str = "";
        switch (atual->op) {
            case OP_ADD: op_str = "ADD"; break;
            case OP_SUB: op_str = "SUB"; break;
            case OP_MULT: op_str = "MULT"; break;
            case OP_DIV: op_str = "DIV"; break;
            case OP_ASSIGN: op_str = "ASSIGN"; break;
            case OP_EQ: op_str = "EQ"; break;
            case OP_LT: op_str = "LT"; break;
            case OP_GT: op_str = "GT"; break;
            case OP_LE: op_str = "LE"; break;
            case OP_NEQ: op_str = "NEQ"; break;
            case OP_GE: op_str = "GE"; break;
            case OP_GOTO: op_str = "GOTO"; break;
            case OP_IF_FALSE: op_str = "IF_FALSE"; break;
            case OP_LABEL: op_str = "LABEL"; break;
            case OP_CALL: op_str = "CALL"; break;
            case OP_PARAM: op_str = "PARAM"; break;
            case OP_RETURN: op_str = "RETURN"; break;
            case OP_VEC_READ: op_str = "VEC_READ"; break;
            case OP_VEC_WRITE: op_str = "VEC_WRITE"; break;
            case OP_HALT: op_str = "HALT"; break;
            case OP_FORMAL_PARAM: op_str = "FORMAL_PARAM"; break;
            default: op_str = "UNKNOWN"; break;    
        }
        
        char* a1 = (atual->arg1 != NULL) ? atual->arg1 : "-";
        char* a2 = (atual->arg2 != NULL) ? atual->arg2 : "-";
        char* res = (atual->result != NULL ) ? atual->result : "-";
        
        printf("(%s, %s, %s, %s)\n", op_str, a1, a2, res);
        atual = atual->prox;
    }
}