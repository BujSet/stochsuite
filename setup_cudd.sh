#!/bin/bash
cd cudd/
autoreconf -fi
./configure
make
