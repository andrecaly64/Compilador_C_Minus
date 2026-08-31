module flags_reg (clk, opcode, Z_in, N_in, Z_out, N_out);
 
input clk;
input [1:0] opcode;
input Z_in, N_in;
output reg Z_out, N_out;
 
always @(negedge clk)
begin
    if (opcode == 2'b00)
    begin
        Z_out <= Z_in;
        N_out <= N_in;
    end
end
 
endmodule
 