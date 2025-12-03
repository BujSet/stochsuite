# RTL Implementations of Pseudo-Random Number Generators

## RNGState Testbench

```
iverilog -g2005-sv RNGState.v RNGState_tb.v -s tb_RNGState -y XilinxUnisimLibrary/verilog/src -o TB-sim
./TB-sim +VCDFILE=sim.vcd +VCDLEVEL=0 | tee sim.log
```

## Taus88 Testbench

```
iverilog RNGState.v Taus88.v Taus88_tb.v -s tb_taus88_core -y XilinxUnisimLibrary/verilog/src -o TB-sim
```
