module mult_in (controle, entrada_in, saida_mult2, saida_io);

input controle;
input [31:0] entrada_in;
input [31:0] saida_mult2;
output reg [31:0] saida_io;

always @(*)
begin
if(controle==1)
saida_io = entrada_in;
else
saida_io = saida_mult2;
end
endmodule
 