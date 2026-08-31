#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cgen.h"

#define MAX_QUADRUPLAS 200
int i;

typedef struct {
    char nome[20];
    int offset;
} variavel;

variavel TABELA_MEMORIA[MAX_QUADRUPLAS];
int total_variaveis = 0;
int offset_atual = 0;
int assign_label_count = 0;
int param_count = 0;
char* lista_parametros[10];
int qtd_parametros = 0;

void reset_memoria() {
    total_variaveis = 0;
    offset_atual = -8;
}

int imediato(char* str) {
    if (str == NULL) {
        return 0;
    }
    if (str[0] == '-' || isdigit(str[0])) {
        return 1;
    }
    return 0;
}

int get_offset(char* nome) {
    if (nome == NULL || imediato(nome)) {
        return 0;
    }
    for(i = 0; i < total_variaveis; i++) {
        if (strcmp(TABELA_MEMORIA[i].nome, nome) == 0) {
            return TABELA_MEMORIA[i].offset;
        }
    }
    
    offset_atual -= 4;
    strcpy(TABELA_MEMORIA[total_variaveis].nome, nome);
    TABELA_MEMORIA[total_variaveis].offset = offset_atual;
    total_variaveis++;

    return offset_atual;
}

void carregar_argumento(char* arg, char* reg) {
    if(imediato(arg)) {
        printf("    SUB %s, %s, %s\n", reg, reg, reg);
        printf("    ADDI %s, %s, #%s\n", reg, reg, arg);
    }
    else {
        int offset = get_offset(arg);
        printf("    LDR %s, [fp, #%d]\n", reg, offset);
    }
}

extern quadrupla* inicio;

void gerador_assembly() {
    quadrupla* atual = inicio;

    printf(".data\n\n");
    printf(".text\n");
    printf(".global main\n\n");

    printf("    MOV sp, #1000\n");
    printf("    B main\n");

    static int formal_param_count = 0;

    while (atual != NULL) {
        switch (atual->op) {
            case OP_LABEL:
                printf("%s:\n", atual->arg1);

                if (strcmp(atual->arg1, "L0") != 0 && atual->arg1[0] != 'L' && atual->arg1[0] != '.') {
                    reset_memoria();
                    formal_param_count = 0;                    
                    
                    // Prólogo de função
                    printf("    SUBI sp, sp, #8\n");
                    printf("    STR fp, [sp, #0]\n");
                    printf("    STR lr, [sp, #4]\n");
                    printf("    ADDI fp, sp, #8\n");
                    printf("    SUBI sp, sp, #80\n");

                    // Inicializa a referência do vetor global na main
                    if (strcmp(atual->arg1, "main") == 0) {
                        int off_vet = get_offset("vet");
                        printf("    SUB r0, r0, r0\n");
                        printf("    ADDI r0, r0, #32\n");
                        printf("    STR r0, [fp, #%d]\n", off_vet);
                    }
                }
                break;

            case OP_FORMAL_PARAM: {
                int offset = get_offset(atual->arg1);
                printf("    STR r%d, [fp, #%d]\n", formal_param_count, offset);
                formal_param_count++;
                break;
            }

            case OP_ASSIGN:
                carregar_argumento(atual->arg1, "r0");
                printf("    STR r0, [fp, #%d]\n", get_offset(atual->result));
                break;

            case OP_ADD:
            case OP_SUB:
            case OP_MULT:
            case OP_DIV:
                carregar_argumento(atual->arg1, "r0");
                carregar_argumento(atual->arg2, "r1");
                
                if (atual->op == OP_ADD)       printf("    ADD r2, r0, r1\n");
                else if (atual->op == OP_SUB)  printf("    SUB r2, r0, r1\n");
                else if (atual->op == OP_MULT) printf("    MUL r2, r0, r1\n");
                else if (atual->op == OP_DIV)  printf("    UDIV r2, r0, r1\n");
                
                printf("    STR r2, [fp, #%d]\n", get_offset(atual->result));
                break;

            case OP_LT:
            case OP_GT:
            case OP_LE:
            case OP_GE:
            case OP_EQ:
            case OP_NEQ: {
                carregar_argumento(atual->arg1, "r0");
                carregar_argumento(atual->arg2, "r1");

                printf("    SUB r2, r0, r1\n");

                int l_true = assign_label_count++;
                int l_end = assign_label_count++;

                if (atual->op == OP_LT)  printf("    BCS\n");
                if (atual->op == OP_GT)  printf("    BLS\n");
                if (atual->op == OP_LE)  printf("    BHI\n");
                if (atual->op == OP_GE)  printf("    BCC\n");
                if (atual->op == OP_EQ)  printf("    BNE\n");
                if (atual->op == OP_NEQ) printf("    BEQ\n");
                printf("    B .L_asm_%d\n", l_true);

                printf("    SUB r2, r2, r2\n");
                printf("    B .L_asm_%d\n", l_end);
                
                printf(".L_asm_%d:\n", l_true);
                printf("    SUB r2, r2, r2\n");
                printf("    ADDI r2, r2, #1\n");
                
                printf(".L_asm_%d:\n", l_end);
                printf("    STR r2, [fp, #%d]\n", get_offset(atual->result));
                break;
            }

            case OP_IF_FALSE:
                carregar_argumento(atual->arg1, "r0");
                printf("    SUB r1, r1, r1\n");
                printf("    SUB r2, r0, r1\n");
                printf("    BNE\n");
                printf("    B %s\n", atual->result);
                break;

            case OP_GOTO:
                printf("    B %s\n", atual->result);
                break;

            case OP_PARAM:
                lista_parametros[qtd_parametros] = atual->arg1;
                qtd_parametros++;
                break;

            case OP_CALL: {
                static int porta_entrada = 1;
                if (strcmp(atual->arg1, "input") == 0) {
                    printf("    BTN\n");
                    if (porta_entrada == 1) {
                        printf("    IN r2\n");
                        porta_entrada = 2; 
                    } else {
                        printf("    IN2 r2\n");
                        porta_entrada = 1; 
                    }
                    printf("    STR r2, [fp, #%d]\n", get_offset(atual->result));
                }
                else if (strcmp(atual->arg1, "output") == 0 ) {
                    if (qtd_parametros > 0) {
                        carregar_argumento(lista_parametros[0], "r0");
                    }
                    printf("    OUT r0\n");
                    qtd_parametros = 0;
                }
                else {
                    for (int p = 0; p < qtd_parametros; p++) {
                        char reg_destino[5];
                        sprintf(reg_destino, "r%d", p);
                        carregar_argumento(lista_parametros[p], reg_destino);
                    }
                    qtd_parametros = 0; 

                    int l_ret = assign_label_count++;
                    
                    printf("    MOV r4, #.L_asm_%d\n", l_ret);
                    printf("    CPY lr, r4\n");
                    printf("    B %s\n", atual->arg1);
                    printf(".L_asm_%d:\n", l_ret);
                    if (atual->result != NULL) {
                        printf("    STR r0, [fp, #%d]\n", get_offset(atual->result));
                    }
                }
                param_count = 0;
                break;
            }

            case OP_RETURN:
                if (atual->arg1 != NULL) {
                    carregar_argumento(atual->arg1, "r0");
                }
                // Restaura lr e o fp antigo
                printf("    LDR lr, [fp, #-4]\n");
                printf("    LDR r4, [fp, #-8]\n");
                
                // sp recupera o topo do frame anterior
                printf("    CPY sp, fp\n");
                printf("    CPY fp, r4\n");
                printf("    CPY r15, r14\n");
                break;

            case OP_VEC_READ:
                carregar_argumento(atual->arg2, "r1");
                printf("    SUB r2, r2, r2\n");
                printf("    ADDI r2, r2, #4\n");
                printf("    MUL r1, r1, r2\n");
                printf("    LDR r0, [fp, #%d]\n", get_offset(atual->arg1));
                printf("    ADD r0, r0, r1\n");
                printf("    LDR r3, [r0, #0]\n");
                printf("    STR r3, [fp, #%d]\n", get_offset(atual->result));
                break;

            case OP_VEC_WRITE:
                carregar_argumento(atual->arg1, "r1");
                printf("    SUB r2, r2, r2\n");
                printf("    ADDI r2, r2, #4\n");
                printf("    MUL r1, r1, r2\n");
                printf("    LDR r0, [fp, #%d]\n", get_offset(atual->result));
                printf("    ADD r0, r0, r1\n");
                carregar_argumento(atual->arg2, "r3");
                printf("    STR r3, [r0, #0]\n");
                break;

            case OP_HALT:
                printf("\n.L_FIM_EXECUCAO:\n");
                printf("    B .L_FIM_EXECUCAO\n");
                break;

            default:
                break;
        }
        atual = atual->prox;
    }
}