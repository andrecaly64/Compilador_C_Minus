module multiplexador_pc (entrada1, entrada2, saida, pc_fonte);

input [9:0] entrada1, entrada2;
output reg [9:0] saida;
input pc_fonte;

always @(*)
case(pc_fonte)
1'b0: saida = entrada1;
1'b1: saida = entrada2;
endcase
endmodule
