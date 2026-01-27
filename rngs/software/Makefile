include $(STOCHSUITE_HOME)/stochsuite.mk

all:
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) RNGState.cpp -o RNGState.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) RandomNumberGenerator.cpp -o RandomNumberGenerator.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) RNGFactory.cpp -o RNGFactory.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) Taus88.cpp -o Taus88.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) Taus113.cpp -o Taus113.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) JKISS.cpp -o JKISS.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) JKISS32.cpp -o JKISS32.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) LCG.cpp -o LCG.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) MersenneTwister.cpp -o MersenneTwister.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) KISS11.cpp -o KISS11.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) PCGBasic.cpp -o PCGBasic.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) -march=armv8.5-a+rng HWRNG.cpp -o HWRNG.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) XorShift32.cpp -o XorShift32.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) XorShift128.cpp -o XorShift128.o
	@$(CPP) -I$(RNG_LIB) $(CFLAGS) XorWow.cpp -o XorWow.o

clean:
	@rm -f *.o
