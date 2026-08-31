module Ula (a, b, sel, r, N, Z);
input [3:0] sel;
input [31:0] a, b;
reg[31:0] r0;
output reg[31:0] r;
output N, Z;
always@(sel or a or b)
begin
case(sel[3:0])
4'b0000:r= ~a;
4'b0001:r= a;
4'b0010:r= a & b;
4'b0011:r= a | b;
4'b0100:r= a + b;
4'b0101:r= a - b;
4'b0110:r= a * b;
4'b0111:r= a / b;
4'b1000:r= ~b;
4'b1001:r= b;
endcase
end
always@(*)
begin
r0 = r;
end
assign N = r[31];
assign Z = (r == 32'b0) ? 1'b1 : 1'b0;
endmodule
