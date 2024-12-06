TARGET=pi

$(TARGET):
	g++ -I/stochsuite/rngs /stochsuite/rngs/RNGState.o /stochsuite/rngs/RandomNumberGenerator.o /stochsuite/rngs/Taus88.o -std=c++11 $(TARGET).cpp  -o $(TARGET).o -Wall -O3

clean:
	rm -f $(TARGET).o
