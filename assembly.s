Analisando o arquivo: teste2.txt
Análise sintática concluída com sucesso.

Árvore sintática gerada:
  TIPO int 
    DECLARACAO DE FUNCAO: gcd
      TIPO int 
        DECLARACAO DE VARIAVEL: u
      TIPO int 
        DECLARACAO DE VARIAVEL: v
      if 
        OPERADOR: == 
          ID: v
          NUM: 0
        return 
          ID: u
      else 
        return 
          CHAMADA DE FUNCAO: gcd
            ID: v
            OPERADOR: - 
              ID: u
              OPERADOR: * 
                OPERADOR: / 
                  ID: u
                  ID: v
                ID: v
  TIPO void 
    DECLARACAO DE FUNCAO: main
      void 
      TIPO int 
        DECLARACAO DE VARIAVEL: x
      TIPO int 
        DECLARACAO DE VARIAVEL: y
      ATRIBUICAO 
        ID: x
        CHAMADA DE FUNCAO: input
      ATRIBUICAO 
        ID: y
        CHAMADA DE FUNCAO: input
      CHAMADA DE FUNCAO: output
        CHAMADA DE FUNCAO: gcd
          ID: x
          ID: y

Tabela de Símbolos:
Name     | Scope        | Tipo ID      | Tipo dado | Linhas
-------------------------------------------------------------
main     | global       | func         | void     | 11 
input    | global       | func         | int      | 13 13 
output   | global       | func         | void     | 14 
u        | gcd          | var          | int      | 7 7 6 4 
v        | gcd          | var          | int      | 7 7 7 6 4 
x        | main         | var          | int      | 14 13 12 
y        | main         | var          | int      | 14 13 12 
gcd      | global       | func         | int      | 14 4 

Código Intermediário:

(LABEL, gcd, -, -)
(EQ, v, 0, _t0)
(IF_FALSE, _t0, -, L0)
(RETURN, u, -, -)
(GOTO, -, -, L1)
(LABEL, L0, -, -)
(PARAM, v, -, -)
(DIV, u, v, _t1)
(MULT, _t1, v, _t2)
(SUB, u, _t2, _t3)
(PARAM, _t3, -, -)
(CALL, gcd, -, _t4)
(RETURN, _t4, -, -)
(LABEL, L1, -, -)
(RETURN, -, -, -)
(LABEL, main, -, -)
(CALL, input, -, _t5)
(ASSIGN, _t5, -, x)
(CALL, input, -, _t6)
(ASSIGN, _t6, -, y)
(PARAM, x, -, -)
(PARAM, y, -, -)
(CALL, gcd, -, _t7)
(PARAM, _t7, -, -)
(CALL, output, -, -)
(HALT, -, -, -)
.data

.text
.global main

    MOV sp, #500
    B main
gcd:
    SUBI sp, sp, #8
    STR fp, [sp, #0]
    STR lr, [sp, #4]
    ADDI fp, sp, #8
    SUBI sp, sp, #24
    STR r0, [fp, #-12]
    STR r1, [fp, #-16]
    LDR r0, [fp, #-16]
    SUB r1, r1, r1
    ADDI r1, r1, #0
    SUB r2, r0, r1
    BNE
    B .L_asm_0
    SUB r2, r2, r2
    B .L_asm_1
.L_asm_0:
    SUB r2, r2, r2
    ADDI r2, r2, #1
.L_asm_1:
    STR r2, [fp, #-20]
    LDR r0, [fp, #-20]
    SUB r1, r1, r1
    SUB r2, r0, r1
    BNE
    B L0
    LDR r0, [fp, #-12]
    LDR lr, [fp, #-4]
    LDR fp, [fp, #-8]
    CPY r15, r14
    B L1
L0:
    LDR r0, [fp, #-12]
    LDR r1, [fp, #-16]
    UDIV r2, r0, r1
    STR r2, [fp, #-24]
    LDR r0, [fp, #-24]
    LDR r1, [fp, #-16]
    MUL r2, r0, r1
    STR r2, [fp, #-28]
    LDR r0, [fp, #-12]
    LDR r1, [fp, #-28]
    SUB r2, r0, r1
    STR r2, [fp, #-32]
    LDR r0, [fp, #-16]
    LDR r1, [fp, #-32]
    MOV r4, #.L_asm_2
    CPY lr, r4
    B gcd
.L_asm_2:
    STR r0, [fp, #-36]
    LDR r0, [fp, #-36]
    LDR lr, [fp, #-4]
    LDR fp, [fp, #-8]
    CPY r15, r14
L1:
    LDR lr, [fp, #-4]
    LDR fp, [fp, #-8]
    CPY r15, r14
main:
    SUBI sp, sp, #8
    STR fp, [sp, #0]
    STR lr, [sp, #4]
    ADDI fp, sp, #8
    SUBI sp, sp, #24
    MOV r0, #32
    STR r0, [fp, #-16]
    IN r2
    STR r2, [fp, #-20]
    LDR r0, [fp, #-20]
    STR r0, [fp, #-24]
    IN2 r2
    STR r2, [fp, #-28]
    LDR r0, [fp, #-28]
    STR r0, [fp, #-32]
    LDR r0, [fp, #-24]
    LDR r1, [fp, #-32]
    MOV r4, #.L_asm_3
    CPY lr, r4
    B gcd
.L_asm_3:
    STR r0, [fp, #-36]
    LDR r0, [fp, #-36]
    OUT r0

.L_FIM_EXECUCAO:
    B .L_FIM_EXECUCAO
