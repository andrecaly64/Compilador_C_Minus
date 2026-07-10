#include "node.h"

extern char *yytext;

treeNode *syntaxTree;
int treeSize = 0;

void printTreeNode(treeNode *tree) {
    if (tree == NULL) return;

    treeSize += 2;
    for (int i = 0; i < treeSize; i++) {
        printf(" ");
    }

    if (tree->node == exp) {
        switch (tree->nodeSubType.exp) {
            case expNum:
                printf("NUM: %d\n", tree->key.value);
                break;
            case expId:
                if (tree->key.name != NULL && strcmp(tree->key.name, "void") == 0) {
                    printf("void \n");
                } else if (tree->key.name != NULL) {
                    printf("ID: %s\n", tree->key.name);
                    //printf(" -> SCOPE: %s\n", tree->scope);
                } else {
                    printf("ID: NULL\n");
                }
                break;
            case expOp:
                switch (tree->key.op) {
                case 14:
                    printf("OPERADOR: + \n");
                    break;
                case 15:
                    printf("OPERADOR: - \n");
                    break;
                case 16:
                    printf("OPERADOR: * \n");
                    break;
                case 17:
                    printf("OPERADOR: / \n");
                    break;
                case 10:
                    printf("OPERADOR: < \n");
                    break;
                case 11:
                    printf("OPERADOR: <= \n");
                    break;
                case 12:
                    printf("OPERADOR: > \n");
                    break;
                case 13:
                    printf("OPERADOR: >= \n");
                    break;
                case 8:
                    printf("OPERADOR: == \n");
                    break;
                case 9:
                    printf("OPERADOR: != \n");
                    break;
                default:
                    printf("OPERADOR: DESCONHECIDO\n");
                }
                break;
            default:
                printf("EXPRESSAO INVALIDA\n");
        }
    } else if (tree->node == stmt) {
        switch (tree->nodeSubType.stmt) {
            case stmtIf:
                printf("if \n");
                break;
            case stmtWhile:
                printf("while \n");
                break;
            case stmtAttrib:
                printf("ATRIBUICAO \n");
                break;
            case stmtReturn:
                printf("return \n");
                break;
            case stmtFunc:
                if (tree->key.name != NULL) {
                    //printf("Function Call: entrei aqui\n");
                    printf("CHAMADA DE FUNCAO: %s\n", tree->key.name);
                }
                break;
            default:
                printf("DECLARACAO INVALIDA \n");
        }
    } else if (tree->node == decl) {
        switch (tree->nodeSubType.decl) {
            case declFunc:
                if (tree->key.name != NULL) {
                    printf("DECLARACAO DE FUNCAO: %s\n", tree->key.name);
                } else {
                    printf("DECLARACAO DE FUNCAO: NULL\n");
                }
                break;
            case declVar:
                if (tree->key.name != NULL) {
                    printf("DECLARACAO DE VARIAVEL: %s\n", tree->key.name);
                } else {
                    printf("DECLARACAO DE VARIAVEL: NULL\n");
                }
                break;
            case declIdType:
                if (tree->type == Integer) {
                    printf("TIPO int \n");
                } else if (tree->type == Array) {
                    printf("TIPO int[] \n");
                } else {
                    printf("TIPO void \n");
                }
                break;
            default:
                printf("DECLARACAO INVALIDA \n");
        }
    }
}

void printSyntaxTree(treeNode *tree) {
    if (tree == NULL) return;

    printTreeNode(tree);

    for (int i = 0; i < CHILD_MAX_NODES; i++) {
        if (tree->child[i] != NULL) {
            if (tree->node == stmt && tree->nodeSubType.stmt == stmtIf && i == 2) {
                // Imprime a indentação atual para alinhar com o "if" (ou levemente indentado)
                for (int j = 0; j < treeSize; j++) {
                    printf(" ");
                }
                printf("else \n");
            }
            printSyntaxTree(tree->child[i]);
        }
    }
    
    // Decrementa a indentação ao sair do nível atual
    treeSize -= 2;

    if (tree->sibling != NULL) {
        printSyntaxTree(tree->sibling);
    }
}