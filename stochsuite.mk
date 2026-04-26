RNG_LIB := $(STOCHSUITE_HOME)/rngs/software
GEM5_PATH := $(STOCHSUITE_HOME)/gem5_sim/gem5
GEM5_LFLAGS := -L$(GEM5_PATH)/util/m5/build/x86/out -lm5
GEM5_CFLAGS := -I$(GEM5_PATH)/include -I$(GEM5_PATH)/util/m5/src/ -DGEM5_ENABLE -static
RNG_SRCS := $(wildcard $(RNG_LIB)/*.cpp) 
CPP := g++
COMMON_CFLAGS := -c -std=c++11 -Wall -O3 -lm
PLATFORM_CFLAGS := 
LFLAGS := -std=c++11 -Wall -O3
UNAME := $(shell uname)
OS := $(shell uname -s)
ARCH := $(shell uname -m)
