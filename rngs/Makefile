RNG_LIB := /stochsuite/rngs

all:
	$(CC) -I$(RNG_LIB) -c -std=c++11 RNGState.cpp -lm -o RNGState.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 RandomNumberGenerator.cpp -lm -o RandomNumberGenerator.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 RNGFactory.cpp -lm -o RNGFactory.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 Taus88.cpp -lm -o Taus88.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 Taus113.cpp -lm -o Taus113.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 JKISS.cpp -lm -o JKISS.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 JKISS32.cpp -lm -o JKISS32.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 LCG.cpp -lm -o LCG.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 MersenneTwister.cpp -lm -o MersenneTwister.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 KISS11.cpp -lm -o KISS11.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 PCGBasic.cpp -lm -o PCGBasic.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 HWRNG.cpp -lm -o HWRNG.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 XorShift32.cpp -lm -o XorShift32.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 XorShift128.cpp -lm -o XorShift128.o -Wall -O3
	$(CC) -I$(RNG_LIB) -c -std=c++11 XorWow.cpp -lm -o XorWow.o -Wall -O3

clean:
	rm -f $(TARGET).o
