Analisando o arquivo: teste10.txt
Análise sintática concluída com sucesso.

Árvore sintática gerada:
  TIPO int 
    DECLARACAO DE FUNCAO: raizInteira
      TIPO int 
        DECLARACAO DE VARIAVEL: n
      TIPO int 
        DECLARACAO DE VARIAVEL: r
      ATRIBUICAO 
        ID: r
        NUM: 0
      while 
        OPERADOR: <= 
          OPERADOR: * 
            OPERADOR: + 
              ID: r
              NUM: 1
            OPERADOR: + 
              ID: r
              NUM: 1
          ID: n
        ATRIBUICAO 
          ID: r
          OPERADOR: + 
            ID: r
            NUM: 1
      return 
        ID: r
  TIPO void 
    DECLARACAO DE FUNCAO: main
      void 
      TIPO int 
        DECLARACAO DE VARIAVEL: x
      ATRIBUICAO 
        ID: x
        CHAMADA DE FUNCAO: input
      CHAMADA DE FUNCAO: output
        CHAMADA DE FUNCAO: raizInteira
          ID: x

Tabela de Símbolos:
Name     | Scope        | Tipo ID      | Tipo dado | Linhas
-------------------------------------------------------------
main     | global       | func         | void     | 10 
input    | global       | func         | int      | 13 
n        | raizInteira  | var          | int      | 5 2 
r        | raizInteira  | var          | int      | 8 6 6 5 5 4 3 
output   | global       | func         | void     | 14 
x        | main         | var          | int      | 14 13 12 
raizInteira | global       | func         | int      | 14 1 

Código Intermediário:

(LABEL, raizInteira, -, -)
(FORMAL_PARAM, n, -, -)
(ASSIGN, 0, -, r)
(LABEL, L0, -, -)
(ADD, r, 1, _t0)
(ADD, r, 1, _t1)
(MULT, _t0, _t1, _t2)
(LE, _t2, n, _t3)
(IF_FALSE, _t3, -, L1)
(ADD, r, 1, _t4)
(ASSIGN, _t4, -, r)
(GOTO, -, -, L0)
(LABEL, L1, -, -)
(RETURN, r, -, -)
(LABEL, main, -, -)
(CALL, input, -, _t5)
(ASSIGN, _t5, -, x)
(PARAM, x, -, -)
(CALL, raizInteira, -, _t6)
(PARAM, _t6, -, -)
(CALL, output, -, -)
(HALT, -, -, -)
.data

.text
.global main

    MOV sp, #1000
    B main
raizInteira:
    SUBI sp, sp, #8
    STR fp, [sp, #0]
    STR lr, [sp, #4]
    ADDI fp, sp, #8
    SUBI sp, sp, #80
    STR r0, [fp, #-12]
    SUB r0, r0, r0
    ADDI r0, r0, #0
    STR r0, [fp, #-16]
L0:
    LDR r0, [fp, #-16]
    SUB r1, r1, r1
    ADDI r1, r1, #1
    ADD r2, r0, r1
    STR r2, [fp, #-20]
    LDR r0, [fp, #-16]
    SUB r1, r1, r1
    ADDI r1, r1, #1
    ADD r2, r0, r1
    STR r2, [fp, #-24]
    LDR r0, [fp, #-20]
    LDR r1, [fp, #-24]
    MUL r2, r0, r1
    STR r2, [fp, #-28]
    LDR r0, [fp, #-28]
    LDR r1, [fp, #-12]
    SUB r2, r0, r1
    BHI
    B .L_asm_0
    SUB r2, r2, r2
    B .L_asm_1
.L_asm_0:
    SUB r2, r2, r2
    ADDI r2, r2, #1
.L_asm_1:
    STR r2, [fp, #-32]
    LDR r0, [fp, #-32]
    SUB r1, r1, r1
    SUB r2, r0, r1
    BNE
    B L1
    LDR r0, [fp, #-16]
    SUB r1, r1, r1
    ADDI r1, r1, #1
    ADD r2, r0, r1
    STR r2, [fp, #-36]
    LDR r0, [fp, #-36]
    STR r0, [fp, #-16]
    B L0
L1:
    LDR r0, [fp, #-16]
    LDR lr, [fp, #-4]
    LDR r4, [fp, #-8]
    CPY sp, fp
    CPY fp, r4
    CPY r15, r14
main:
    SUBI sp, sp, #8
    STR fp, [sp, #0]
    STR lr, [sp, #4]
    ADDI fp, sp, #8
    SUBI sp, sp, #80
    SUB r0, r0, r0
    ADDI r0, r0, #32
    STR r0, [fp, #-12]
    BTN
    IN r2
    STR r2, [fp, #-16]
    LDR r0, [fp, #-16]
    STR r0, [fp, #-20]
    LDR r0, [fp, #-20]
    MOV r4, #.L_asm_2
    CPY lr, r4
    B raizInteira
.L_asm_2:
    STR r0, [fp, #-24]
    LDR r0, [fp, #-24]
    OUT r0

.L_FIM_EXECUCAO:
    B .L_FIM_EXECUCAO
