include $(STOCHSUITE_HOME)/stochsuite.mk

DUT ?= taus88
XILINX_CELLS_DIR ?= XilinxUnisimLibrary/verilog/src
IVERILOG ?= /usr/bin/iverilog
YOSYS ?= /usr/bin/yosys

sim: ${DUT}-sim
	./$< +VCDFILE=sim.vcd +VCDLEVEL=0 | tee ${DUT}-sim.log

${DUT}-sim: ${DUT}_tb.v ${DUT}.v
	${IVERILOG} -g2005-sv -s ${DUT}_tb -y ${XILINX_CELLS_DIR} -o $@ $^

synth: ${DUT}.netlist.v

${DUT}.netlist.v: ${DUT}.v
	${YOSYS} -p 'synth_xilinx -top ${DUT}' -p stat $< -L ${DUT}-synth.log -o $@

gatesim: ./${DUT}-gatesim
	./$< +VCDFILE=gatesim.vcd +VCDLEVEL=1 | tee ${DUT}-gatesim.log

${DUT}-gatesim: ${DUT}_tb.v ${DUT}.netlist.v
	${IVERILOG} -g2005-sv -s ${DUT}_tb -DGATESIM -y ${XILINX_CELLS_DIR} -y ${XILINX_CELLS_DIR}/unisims -o $@ $^

clean:
	@rm -f *-sim
	@rm -f *-gatesim
	@rm -f *-sim.log
	@rm -f *-synth.log
	@rm -f *-gatesim.log
	@rm -f *.netlist.v
