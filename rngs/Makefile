TARGET=Taus88

all:
	$(CC) -c -std=c++11 RNGState.cpp -lm -o RNGState.o -Wall -O3
	$(CC) -I/stochsuite/rngs -c -std=c++11 RandomNumberGenerator.cpp -lm -o RandomNumberGenerator.o -Wall -O3
	$(CC) -c -std=c++11 RNGFactory.cpp -lm -o RNGFactory.o -Wall -O3
	$(CC) -c -std=c++11 Taus88.cpp -lm -o Taus88.o -Wall -O3


$(TARGET):
	$(CC) -c -std=c++11 $(TARGET).cpp -lm -o $(TARGET).o -Wall -O3

clean:
	rm -f $(TARGET).o
