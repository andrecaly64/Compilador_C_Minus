module PC (clk, prox_endereco, pc, reset);
 
input clk;
input reset;
input wire [31:0] prox_endereco;
output reg[31:0] pc;
 
always @(posedge clk)
begin
if(reset)
    pc <= 32'b0;
else
    pc <= prox_endereco;
end
endmodule