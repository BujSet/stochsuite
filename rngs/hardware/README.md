# RTL Implementations of Pseudo-Random Number Generators

## Simulating Taus88 Testbench

The testbench can be compiled and run with the following command
```
iverilog taus88.v taus88_tb.v -s Taus88_TB -y XilinxUnisimLibrary/verilog/src -o TB-sim && ./TB-sim +VCDFILE=sim.vcd +VCDLEVEL=0 | tee sim.log
```

The hardware testbench checks against values produced from the software implementation, i.e. output from: 

```
./util/correctness.o -iters 10 -seed 0xCAFEBABE -rng Taus88
```

## Synthesizing Taus88 to Netlist

The following command will attempt to use Xilinx primitives to create a netlist to implement design

```
yosys -p 'synth_xilinx -top taus88' -p stat taus88.v -o taus88.netlist.v | tee  synth.log
```

You should see output like:

```
=== design hierarchy ===

   taus88                            1
     $paramod$9306eb01cdb2b5a1b484cfd27f23be9be0d237b3\Register      1

   Number of wires:                115
   Number of wire bits:            741
   Number of public wires:          19
   Number of public wire bits:     645
   Number of memories:               0
   Number of memory bits:            0
   Number of processes:              0
   Number of cells:                320
     FDRE                           96
     LUT2                          132
     LUT3                           79
     LUT4                           13
```

## Functionality of Synthesizable Desgin

If the design were to be routed onto a real FPGA, we would like to verify that 
the design functionally completes from it's netlist implementation. The following 
commands rerun the simulation using the netlist output as the DUT:

```
iverilog -g2005-sv -s Taus88_TB -DGATESIM -y XilinxUnisimLibrary/verilog/src -y XilinxUnisimLibrary/verilog/src/unisims taus88.netlist.v taus88_tb.v -o Taus88-gatesim
./Taus88-gatesim +VCDFILE=gatesim.vcd +VCDLEVEL=1 | tee gatesim.log
```


## Testing with Optimized Taus88

TODO seems like this implementation is broken

```
iverilog taus88_opt.v taus88_opt_tb.v -s Taus88_Optimized_TB -y XilinxUnisimLibrary/verilog/src -o TB-sim && ./TB-sim +VCDFILE=sim.vcd +VCDLEVEL=0 | tee sim.log
yosys -p 'synth_xilinx -top taus88_opt' -p stat taus88_opt.v -o taus88_opt.netlist.v | tee  synth.log
```