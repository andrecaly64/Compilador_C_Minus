module ula_pc (entrada1, saida_pc, clk, reset, pulso_fim);
input [31:0] entrada1;
input reset;
output reg [31:0] saida_pc;
output reg pulso_fim;
reg [1:0] contador = 1;
input clk;
 
always @(posedge clk)
begin
if(reset)
begin
    contador  <= 1;
    saida_pc  <= 32'b0;
    pulso_fim <= 1'b0;
end
else if(contador==2)
begin
saida_pc <= entrada1 + 1;
contador <= 1;
pulso_fim <= 1'b1;  
end
else
begin
contador <= contador + 1;
pulso_fim <= 1'b0;
end
end
endmodule