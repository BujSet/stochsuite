set_cmd_units -time ps -capacitance ff -current mA -voltage V -resistance kOhm

# OpenSTA script (run_sta.tcl)
read_liberty vsclib013.lib
read_verilog lcg_synthesized_netlist.v
link_design lcg

# 3. Define your clock constraint (using 4200 ps for a 4.2 ns clock cycle)
create_clock -name my_clk -period 420000 [get_ports clk]

report_checks -path_delay max -fields {slew cap input pin delay attr}
exit

