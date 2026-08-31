module out (entrada, controle, clock, d0, d1, d2, d3, reset, d4, d5, d6, d7);

input wire [31:0] entrada;
input clock, controle, reset;
output reg [3:0] d0, d1, d2, d3, d4, d5, d6, d7;

reg [31:0] valor_travado = 0;

always @(posedge clock) begin
    if (controle) begin
        valor_travado <= entrada;
    end
	 else if (reset) begin
		valor_travado <= 32'b0;
	 end
end

always @(*) begin
    d0 = (valor_travado % 10);
    d1 = (valor_travado % 100) / 10;
    d2 = (valor_travado % 1000) / 100;
    d3 = (valor_travado % 10000) / 1000;
	 d4 = (valor_travado % 100000) / 10000;
	 d5 = (valor_travado % 1000000) / 100000;
	 d6 = (valor_travado % 10000000) / 1000000;
	 d7 = (valor_travado % 100000000) / 10000000;
	 end

endmodule