module logica_memoria_de_dados (entrada, saida1, saida2, escrita_mem);

input [31:0] entrada;
input escrita_mem;
output reg [31:0] saida1, saida2;

always @(*)
begin
if(escrita_mem == 1'b0)
begin
saida1 = entrada;
end
if(escrita_mem == 1'b1)
begin
saida2 = entrada;
end
end
endmodule
