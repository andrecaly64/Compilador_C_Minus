module banco_de_registradores (rd, rn, rm, r1, r2, sinal_escrita, dado, r3, clk);
    input [4:0] rd, rn, rm;
    input [31:0] dado;
    input sinal_escrita, clk;
    output [31:0] r1, r2, r3;
    
    reg [31:0] memoria [31:0];
    
    integer i;
    
    initial begin
        for (i = 0; i < 32; i = i + 1) begin
            memoria[i] = 32'b0;
        end
        memoria[1]  = 32'd2;
    end

    always @(posedge clk) begin
        if (sinal_escrita) begin
            memoria[rd] <= dado; 
        end
    end
    
    assign r1 = memoria[rn];
    assign r2 = memoria[rm];
    assign r3 = memoria[rd];
    
endmodule