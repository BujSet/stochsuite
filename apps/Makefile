include $(STOCHSUITE_HOME)/stochsuite.mk
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o) 

all:
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) pi.cpp -o pi.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) correctness.cpp -o correctness.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dropout.cpp -o dropout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dop.cpp -o dop.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) photon.cpp -o photon.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) tailwag.cpp -o tailwag.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) to_stdout.cpp -o to_stdout.o

clean:
	@rm -f *.o
	@rm -f *.ppm
