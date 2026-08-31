module debounce (clk, botao_bruto, pulso_pressionado);

parameter LIMIAR_DEBOUNCE = 24'd10000000;
parameter LARGURA_CONTADOR = 10;  

input clk;
input botao_bruto;
output reg pulso_pressionado;

reg sync1 = 1'b0, sync2 = 1'b0;
reg [LARGURA_CONTADOR-1:0] contador = {LARGURA_CONTADOR{1'b0}};
reg estavel = 1'b0;
reg estavel_anterior = 1'b0;

always @(posedge clk)
begin
    sync1 <= botao_bruto;
    sync2 <= sync1;

    if (sync2 == estavel)
    begin
        contador <= {LARGURA_CONTADOR{1'b0}};
    end
    else
    begin
        contador <= contador + 1'b1;
        if (contador == LIMIAR_DEBOUNCE)
        begin
            estavel <= sync2;
        end
    end

    estavel_anterior <= estavel;
    pulso_pressionado <= estavel & ~estavel_anterior;
end

endmodule