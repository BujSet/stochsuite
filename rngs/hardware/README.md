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

### Troubleshooting

If you get an error like:

```
taus88.netlist.v:1731: error: Unknown module type: FDRE
taus88.netlist.v:1742: error: Unknown module type: FDRE
389 error(s) during elaboration.
*** These modules were missing:
        BUFG referenced 1 times.
        FDRE referenced 96 times.
        IBUF referenced 35 times.
        INV referenced 96 times.
        LUT2 referenced 36 times.
        LUT3 referenced 79 times.
        LUT4 referenced 13 times.
        OBUF referenced 32 times.
***
```

It's likely that the `XilinxUnisimLibrary` was not pulled. Run the following and then retry
the command:

```
cd $STOCHSUITE_HOME
git submodule update --init --recursive
cd $STOCHSUITE_HOME/rngs/hardware/
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
     $paramod$e07d4e29b3300cbec84577cedf54b2f8181665cb\Register      1

   Number of wires:                183
   Number of wire bits:            887
   Number of public wires:          19
   Number of public wire bits:     645
   Number of memories:               0
   Number of memory bits:            0
   Number of processes:              0
   Number of cells:                388
     BUFG                            1
     FDRE                           96
     IBUF                           35
     INV                            96
     LUT2                           36
     LUT3                           79
     LUT4                           13
     OBUF                           32
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

## Testing JKISS

```
./util/correctness.o -iters 10 -seed 0xdeadbeef -rng JKISS
iverilog jkiss.v jkiss_tb.v -s JKISS_TB -y XilinxUnisimLibrary/verilog/src -o JKISS-sim && ./JKISS-sim +VCDFILE=sim.vcd +VCDLEVEL=0 | tee sim.log
```
