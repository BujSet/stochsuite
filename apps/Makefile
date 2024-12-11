RNG_LIB := /stochsuite/rngs
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o) 

all:
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 pi.cpp -o pi.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 correctness.cpp -o correctness.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 dropout.cpp -o dropout.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 dop.cpp -o dop.o -Wall -O3

clean:
	rm -f $(TARGET).o
