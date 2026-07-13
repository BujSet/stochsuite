#!/bin/bash
cd OpenSTA/
mkdir build
cd build
cmake -DCUDD_DIR=/root/stochsuite/cudd ..
make
