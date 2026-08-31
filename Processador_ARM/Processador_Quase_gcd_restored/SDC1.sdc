create_clock -period 100.000 -name clk [get_ports {clk}]

create_clock -name clk0 -period 20.000 [get_ports {clk0}]

derive_pll_clocks
derive_clock_uncertainty