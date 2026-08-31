module multiplexador1 (sel, entrada1, entrada2, saida);

input [31:0] entrada1, entrada2;
input sel;
output reg [31:0] saida;

always @(*)
case (sel)
1'b0: saida = entrada1;
1'b1: saida = entrada2;
endcase
endmodule
