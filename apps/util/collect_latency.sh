#!/bin/bash

./util/latency.o -rng UpCounter -header && \
        ./util/latency.o -rng CONG &&  \
        ./util/latency.o -rng GLIBC_CRAND &&  \
        ./util/latency.o -rng DRAND48 &&  \
        ./util/latency.o -rng Taus88 &&  \
        ./util/latency.o -rng Taus113 &&  \
        ./util/latency.o -rng JKISS &&  \
        ./util/latency.o -rng JKISS32 &&  \
        ./util/latency.o -rng MersenneTwister &&  \
        ./util/latency.o -rng KISS11 &&  \
        ./util/latency.o -rng XorShift32 && \
        ./util/latency.o -rng XorShift128 && \
        ./util/latency.o -rng XorWow && \
        ./util/latency.o -rng XoShiRo128++ && \
        ./util/latency.o -rng ParkAndMiller1988Int2 && \
        ./util/latency.o -rng HWRNG
