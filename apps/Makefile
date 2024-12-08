all:
	g++ -I/stochsuite/rngs /stochsuite/rngs/RNGState.o /stochsuite/rngs/RandomNumberGenerator.o /stochsuite/rngs/Taus88.o -std=c++11 pi.cpp  -o pi.o -Wall -O3
	g++ -I/stochsuite/rngs /stochsuite/rngs/RNGState.o /stochsuite/rngs/RandomNumberGenerator.o /stochsuite/rngs/Taus88.o -std=c++11 correctness.cpp  -o correctness.o -Wall -O3

clean:
	rm -f $(TARGET).o
