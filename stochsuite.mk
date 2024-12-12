RNG_LIB := $(STOCHSUITE_HOME)/rngs
RNG_SRCS := $(wildcard $(RNG_LIB)/*.cpp) 
CPP := g++
CFLAGS := -c -std=c++11 -Wall -O3 -lm
LFLAGS := -std=c++11 -Wall -O3