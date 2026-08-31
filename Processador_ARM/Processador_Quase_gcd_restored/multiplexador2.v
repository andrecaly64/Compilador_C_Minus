module multiplexador2 (entrada1, entrada2, saida, sel);

input [31:0] entrada1, entrada2;
output reg [31:0] saida;
input sel;

always @(*)
case(sel)
1'b0: saida = entrada1;
1'b1: saida = entrada2;
endcase
endmodule
