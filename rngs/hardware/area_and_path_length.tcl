# 1. Read and Elaborate
yosys read -sv "$::env(MODULE).v"
yosys hierarchy -top $::env(MODULE)

# 2. Generic Synthesis (Fixes path/logic issues)
yosys "synth -top $::env(MODULE)"

# 3. Map to your specific Library (Fixes Area)
yosys "dfflibmap -liberty vsclib013.lib"
yosys "abc -liberty vsclib013.lib"

# 4. Clean and Report
yosys clean
yosys "stat -liberty vsclib013.lib"
yosys "ltp -noff"
