module extensor_bit (sel, entrada1, entrada2, entrada3, entrada4, saida);

input [1:0] sel;
input [9:0] entrada1;
input [23:0] entrada2;
input [7:0] entrada3;
input [7:0] entrada4;
output reg [31:0] saida;

always @(*)
begin
case (sel)
2'b00: begin
saida = entrada1;
if(entrada1[9] == 1'b1) saida = saida + 32'b11111111111111111111110000000000;
end
2'b01: begin
saida = entrada2;
if(entrada2[23] == 1'b1) saida = saida + 32'b11111111100000000000000000000000;
end
2'b10: begin
saida = entrada3;
if(entrada3[7] == 1'b1) saida = saida + 32'b111111111111111111111111100000000;
end
2'b11: begin
saida = entrada4;
if(entrada4[7] == 1'b1) saida = saida + 32'b111111111111111111111111100000000;
end
endcase
end
endmodule
