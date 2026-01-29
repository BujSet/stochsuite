RNG_LIB := $(STOCHSUITE_HOME)/rngs/software
RNG_SRCS := $(wildcard $(RNG_LIB)/*.cpp) 
CPP := g++
COMMON_CFLAGS := -c -std=c++11 -Wall -O3 -lm
PLATFORM_CFLAGS := 
LFLAGS := -std=c++11 -Wall -O3
UNAME := $(shell uname)
