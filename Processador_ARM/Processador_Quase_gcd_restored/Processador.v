module Processador(saida_ula, escrita_reg, operacao, negativo, positivo, endereco_escrita, endereco_leitura, escrita_mem, operando_ula, /*saida_mult3,*/ 
clk, instrucao, zero, dado_escrita, /*saida_mult2,*/ rn, rm, r3, rd, cond, sinal_mult2, endereco_pc, prox_pc, prox_pc_final, clk0, reset, saida_reg2, saida_reg1,
pc_fonte, fonte_reg, /*operando2, saida_extensor,*/ reg_display1, reg_display0, reg_display2, reg_display3, imm8, imm8_2, entrada_io, controle_out, dado_leitura,
seg0, seg1, seg2, seg3, dado_reg, imediato, botao_bruto, botao_pressionado, aguarda_botao, pulso_fim, travado, contador2, key, seg4, seg5, seg6, seg7, reg_display4, reg_display5, reg_display6, reg_display7);
input clk0, reset;
output clk;
output botao_pressionado;
input botao_bruto;
output travado;
input key;
wire[1:0] contador;
output reg [1:0] contador2;
input [7:0] imm8, imm8_2;
output escrita_reg, sinal_mult2, escrita_mem, operando_ula, fonte_reg, pc_fonte, entrada_io, controle_out, imediato;
output [3:0] operacao;
output [31:0] instrucao, endereco_escrita, endereco_leitura;
output [31:0] saida_ula;
output negativo, positivo, zero;
wire [31:0] operando2, saida_extensor;
output wire [31:0] saida_reg2, saida_reg1;
output reg [4:0]  rd, cond;
output reg [4:0] rn, rm;
reg [1:0] op;
reg [5:0] funct;
output wire [31:0] dado_escrita;
output wire [31:0] dado_leitura;
wire [31:0] saida_mult2, saida_mult3;
wire [1:0] imediato;
reg [9:0] imm10;
reg [23:0] imm24;
output [31:0] r3, dado_reg;
output [9:0] endereco_pc, prox_pc, prox_pc_final;
output [3:0] reg_display1, reg_display0, reg_display2, reg_display3, reg_display4, reg_display5, reg_display6, reg_display7;
output [6:0] seg0, seg1, seg2, seg3, seg4, seg5, seg6, seg7;
output wire aguarda_botao;

always@(*)
begin
contador2 = contador;
end

wire zero_travado, negativo_travado;
 
wire [9:0] pular_endereco;
wire [9:0] alvo_desvio;
 
output wire pulso_fim;
wire escrita_reg_real;

wire retorno_pc;
assign retorno_pc = (op == 2'b00) && (funct == 6'b000010) && (rd == 5'd15);

assign travado = aguarda_botao & (botao_bruto);
 
wire pc_fonte_real;
wire [9:0] alvo_final;
assign pc_fonte_real = pc_fonte | retorno_pc | travado;
assign alvo_final = travado ? endereco_pc : (retorno_pc ? saida_ula[9:0] : alvo_desvio);
 
assign pular_endereco = prox_pc + 10'd1;
assign alvo_desvio = (cond == 4'b1110) ? imm10 : pular_endereco;

wire precisa_gate_reg = (op == 2'b00) && (funct != 6'b111111);
assign escrita_reg_real = precisa_gate_reg ? (escrita_reg & pulso_fim) : escrita_reg;

always @(*)
begin
rm = instrucao[9:5];
rn = instrucao[19:15];
rd = instrucao[14:10];
cond = instrucao[31:28];
op = instrucao[27:26];
funct = instrucao[25:20];
imm10 = instrucao[9:0];
imm24 = instrucao[23:0];
end
contato1 (.clk0(clk0),.key(key),.clk(clk));
banco_de_registradores A1(.clk(clk),.sinal_escrita(escrita_reg_real),.r1(saida_reg1),.r2(saida_reg2),.dado(dado_reg),.rn(rn),.rd(rd),.rm(saida_mult3),.r3(r3));
Ula T2(.r(saida_ula),.sel(operacao),.N(negativo),.Z(zero),.a(saida_reg1),.b(operando2));
memoria_de_instrucoes B1(.clk(clk),.q(instrucao),.addr(endereco_pc));
memoria_de_dados C1(.clk(clk),.q(dado_leitura),.data(saida_reg2),.read_addr(endereco_leitura),.write_addr(endereco_escrita),.we(escrita_mem));
multiplexador1 D1(.entrada1(saida_reg2),.saida(operando2),.entrada2(saida_extensor),.sel(operando_ula));
extensor_bit E1(.saida(saida_extensor),.entrada1(imm10),.entrada2(imm24),.entrada3(imm8),.entrada4(imm8_2),.sel(imediato));
multiplexador2 F1(.entrada1(saida_ula),.entrada2(dado_leitura),.saida(saida_mult2),.sel(sinal_mult2));

flags_reg FLAGS1(.clk(clk),.opcode(op),.Z_in(zero),.N_in(negativo),.Z_out(zero_travado),.N_out(negativo_travado));

unidade_de_controle G1(.funct(funct),.opcode(op),.cond(cond),.N(negativo_travado),.Z(zero_travado),.OperacaoULA(operacao),.PCFonte(pc_fonte),.entrada_io(entrada_io),
.Imediato(imediato),.EscritaReg(escrita_reg),.MemToReg(sinal_mult2),.EscritaMem(escrita_mem),.OperandoULA(operando_ula),.RegFonte(fonte_reg),
.controle_out(controle_out),.aguarda_botao(aguarda_botao));

logica_memoria_de_dados H1(.escrita_mem(escrita_mem),.entrada(saida_ula),.saida1(endereco_leitura),.saida2(endereco_escrita));
multiplexador3 I1(.sinal_controle_reg(fonte_reg),.entrada1(rm),.entrada2(rd),.saida(saida_mult3));
PC J1(.pc(endereco_pc),.clk(clk),.prox_endereco(prox_pc_final),.reset(reset));

ula_pc K1(.entrada1(endereco_pc),.saida_pc(prox_pc),.clk(clk),.pulso_fim(pulso_fim),.reset(reset));

multiplexador_pc L1(.entrada1(prox_pc),.entrada2(alvo_final),.saida(prox_pc_final),.pc_fonte(pc_fonte_real));
out M1(.entrada(saida_mult2),.clock(clk),.d0(reg_display0),.d1(reg_display1),.d2(reg_display2),.d3(reg_display3),.controle(controle_out),.reset(reset));
mult_in N1(.controle(entrada_io),.entrada_in(saida_extensor),.saida_io(dado_reg),.saida_mult2(saida_mult2));

debounce BTN1(.clk(clk0),.botao_bruto(botao_bruto),.pulso_pressionado(botao_pressionado));

bcd chamada0 (.bcd(reg_display0),.seg(seg0));
bcd chamada1 (.bcd(reg_display1),.seg(seg1));
bcd chamada2 (.bcd(reg_display2),.seg(seg2));
bcd chamada3 (.bcd(reg_display3),.seg(seg3));
bcd chamada4 (.bcd(reg_display4),.seg(seg4));
bcd chamada5 (.bcd(reg_display5),.seg(seg5));
bcd chamada6 (.bcd(reg_display6),.seg(seg6));
bcd chamada7 (.bcd(reg_display7),.seg(seg7));
endmodule