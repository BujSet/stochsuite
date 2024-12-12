RNG_LIB := $(HOME)stochsuite/rngs
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o) 

all:
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 pi.cpp -o pi.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 correctness.cpp -o correctness.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 dropout.cpp -o dropout.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 dop.cpp -o dop.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 photon.cpp -o photon.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 tailwag.cpp -o tailwag.o -Wall -O3
	g++ -I$(RNG_LIB) $(RNG_OBJS) -std=c++11 to_stdout.cpp -o to_stdout.o -Wall -O3

clean:
	rm -f $(TARGET).o
