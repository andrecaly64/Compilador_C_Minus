module multiplexador3 (entrada1, entrada2, saida, sinal_controle_reg);

input [4:0] entrada1, entrada2;
output reg [4:0] saida;
input sinal_controle_reg;

always @(*)
case(sinal_controle_reg)
1'b0: saida = entrada1;
1'b1: saida = entrada2;
endcase
endmodule
