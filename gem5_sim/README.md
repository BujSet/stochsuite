# Instructions to Build and Run Applications with gem5

## Ensure gem5 repo is available

Make sure that all submodules were pulled and updated. Run the follwoing from 
top level directory:

```
git submodule update --init --remote --recursive
```

## Build gem5

```
cd $STOCHSUITE_HOME/gem5_sim/gem5/
python3 `which scons` build/X86/gem5.opt -j9
```

## Build m5 operators

```
cd $STOCHSUITE_HOME/gem5_sim/gem5/util/m5/
scons build/x86/out/m5
```
