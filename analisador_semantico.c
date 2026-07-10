#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "analisador_semantico.h"
#include "symbol_table.h"

extern SymbolTable tabela;
extern treeNode *syntaxTree;

static char *currentScope = "global";

// Função movida para o topo 
static int countArguments(treeNode *args) {
    int count = 0;
    treeNode *t = args;
    while (t != NULL) {
        count++;
        t = t->sibling;
    }
    return count;
}

// Estrutura para rastrear declarações no escopo atual durante a análise
typedef struct DeclaredVar {
    char *name;
    char *scope;
    struct DeclaredVar *next;
} DeclaredVar;

static DeclaredVar *declaredList = NULL;

// Função para verificar se já foi declarado no escopo
static int wasDeclared(char *name, char *scope) {
    DeclaredVar *current = declaredList;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->scope, scope) == 0) {
            return 1; // Encontrou duplicata
        }
        current = current->next;
    }
    return 0; // Não encontrado
}

// Função para registrar uma nova declaração
static void registerDeclaration(char *name, char *scope) {
    DeclaredVar *newNode = (DeclaredVar *)malloc(sizeof(DeclaredVar));
    newNode->name = strdup(name);
    newNode->scope = strdup(scope);
    newNode->next = declaredList;
    declaredList = newNode;
}

static void traverse(treeNode *t);

static Symbol* lookupSymbol(char *name) {
    Symbol *sym = findSymbol(&tabela, name, currentScope);
    
    if (sym == NULL && strcmp(currentScope, "global") != 0) {
        sym = findSymbol(&tabela, name, "global");
    }
    
    return sym;
}

static void semanticError(treeNode *t, char *msg) {
    if (t != NULL)
        fprintf(stderr, "(!) ERRO SEMANTICO: %s | Linha %d\n", msg, t->line);
    else
        fprintf(stderr, "(!) ERRO SEMANTICO: %s\n", msg);
}

static void checkNode(treeNode *t) {
    Symbol *sym;
    Symbol *funcSym;

    switch (t->node) {
        
        case exp:
            switch (t->nodeSubType.exp) {
                case expOp:
                    if ((t->child[0]->type != Integer) || (t->child[1]->type != Integer)) {
                        semanticError(t, "Operacao requer operandos do tipo Integer");
                    }
                    
                    t->type = Integer; 
                    break;

                case expNum:
                    t->type = Integer;
                    break;

                case expId:
                    if (t->key.name != NULL && strcmp(t->key.name, "void") == 0) {
                        t->type = Void; 
                        break; 
                    }
                    sym = lookupSymbol(t->key.name);
                    if (sym == NULL) {
                        char erro[100];
                        sprintf(erro, "Variavel '%s' nao declarada", t->key.name);
                        semanticError(t, erro);
                        t->type = Integer; 
                    } else {
                        // Verifica se é um acesso a array
                        if (t->child[0] != NULL) {
                            t->type = Integer;
                            // Verificar se o índice é inteiro
                            if (t->child[0]->type != Integer) {
                                semanticError(t->child[0], "Indice do array deve ser inteiro");
                            }                        
                            // Verificar se a variável é realmente um array
                            if (sym->type != ARRAY) {
                                char erro[150];
                                sprintf(erro, "Variavel '%s' nao e um array", t->key.name);
                                semanticError(t, erro);
                            }
                        } 
                        else {
                            // É apenas o identificador
                            if (sym->type == ARRAY) {
                                t->type = Array;
                            } else {
                                t->type = sym->dataType;
                            }
                        }
                    }
                    break;
            }
            break;

        case stmt:
            switch (t->nodeSubType.stmt) {
                case stmtIf:
                case stmtWhile:
                    if (t->child[0]->type == Void) {
                        semanticError(t->child[0], "Condicao do IF/WHILE nao pode ser Void");
                    }
                    break;

                case stmtAttrib:
                    if (t->child[0]->nodeSubType.exp == expId) {
                        sym = lookupSymbol(t->child[0]->key.name);
                        if (sym == NULL) {
                        } else {
                            if (t->child[0]->type != t->child[1]->type) {
                                semanticError(t, "Atribuicao com tipos incompatíveis");
                            }
                            if (sym->type == ARRAY && t->child[0]->child[0] == NULL) {
                                semanticError(t, "Atribuicao direta para Array nao permitida (use indices)");
                            }
                        }
                    }
                    break;

        case stmtFunc:
                sym = findSymbol(&tabela, t->key.name, "global");
                if (sym == NULL) {
                       char erro[100];
                    sprintf(erro, "Funcao '%s' nao declarada", t->key.name);
                    semanticError(t, erro);
                } else {
                    t->type = sym->dataType;
                        
                    // Problema 8: Contagem de Argumentos 
                    int numArgs = countArguments(t->child[1]); 
                    
                    if (numArgs != sym->numParams) {
                         char erro[150];
                         sprintf(erro, "Chamada invalida para '%s': esperava %d argumento(s), recebeu %d", 
                         t->key.name, sym->numParams, numArgs);
                         semanticError(t, erro);
                    } 
                    else {
                         // Problema 9: Verificação de Tipos 
                         treeNode *argNode = t->child[1];
                         int i = 0;
                             
                         while (argNode != NULL && i < sym->numParams) {
                            if (argNode->type != sym->paramTypes[i]) {
                                char erro[150];
                                sprintf(erro, 
                                    "Tipo de argumento invalido na chamada de '%s'. Argumento %d esperava %s, recebeu %s", 
                                       t->key.name, 
                                       i + 1, 
                                       (sym->paramTypes[i] == Array ? "Array" : "Integer"),
                                       (argNode->type == Array ? "Array" : "Integer")
                                   );
                                   semanticError(argNode, erro);
                            }
                            argNode = argNode->sibling;
                            i++;
                         }
                    }                        
                    // Problema 10: Retorno Ignorado 
                   if (t->isIgnored && sym->dataType != Void) {
                        semanticError(t, "chamada invalida, parametro de retorno da funcao nao previsto");
                    }
                }
                break;

                case stmtReturn:
                    if (strcmp(currentScope, "global") == 0) {
                        semanticError(t, "Return fora de funcao");
                    } else {
                        funcSym = findSymbol(&tabela, currentScope, "global");
                        
                        if (t->child[0] != NULL) {
                            if (funcSym->dataType == Void)
                                semanticError(t, "Funcao Void nao pode retornar valor");
                            else if (t->child[0]->type != funcSym->dataType)
                                semanticError(t, "Tipo de retorno incompativel com a funcao");
                        } 
                        else {
                            if (funcSym->dataType != Void)
                                semanticError(t, "Funcao nao-Void deve retornar um valor");
                        }
                    }
                    break;
            }
            break;

        case decl:
            switch (t->nodeSubType.decl) {
                case declVar: //Verificação Erro 3 de Variável Void
                    if (t->type == Void) {
                        semanticError(t, "Variavel nao pode ser declarada como Void");
                    }
                    // Verificação Erro 7
                    Symbol *symGlobal = findSymbol(&tabela, t->key.name, "global");
                    if (symGlobal != NULL && symGlobal->type == FUNC) {
                        char erro[100];
                        sprintf(erro, "Declaracao invalida, '%s' ja foi declarado como nome de funcao", t->key.name);
                        semanticError(t, erro);
                    } 
                    //Verificação Erro 4:
                    if (wasDeclared(t->key.name, currentScope)) {
                        char erro[200];
                        sprintf(erro, "Declaracao invalida de variavel, '%s' ja foi declarada previamente", t->key.name);
                        semanticError(t, erro);
                    } else {
                        // Se não foi declarada, registra agora
                        registerDeclaration(t->key.name, currentScope);
                    }
                    break;
            }
            break;
    }
}

static void traverse(treeNode *t) {
    if (t != NULL) {
        
        char *savedScope = currentScope; 
        
        if (t->node == decl && t->nodeSubType.decl == declFunc) {
            currentScope = t->key.name;
        }

        int i;
        for (i = 0; i < CHILD_MAX_NODES; i++) {
            traverse(t->child[i]);
        }

        checkNode(t);

        currentScope = savedScope;

        traverse(t->sibling);
    }
}

void semanticAnalysis(treeNode *tree) {
    //Verificação da entrada de output()
    if (findSymbol(&tabela, "output", "global") != NULL) {
        primitiveType outputParams[] = { Integer };
        setSymbolParams(&tabela, "output", "global", 1, outputParams);
    }
    if (findSymbol(&tabela, "main", "global") == NULL) {
        fprintf(stderr, "ERRO SEMANTICO: Funcao 'main' nao encontrada.\n");
    }
    traverse(tree);
}

