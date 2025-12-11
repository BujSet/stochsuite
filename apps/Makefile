include $(STOCHSUITE_HOME)/stochsuite.mk
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o) 

all:
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) pi/pi.cpp -o pi/pi.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/correctness.cpp -o util/correctness.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dropout/dropout.cpp -o dropout/dropout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) multinomial/multinomial.cpp -o multinomial/multinomial.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dop/dop.cpp -o dop/dop.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) photon/photon.cpp -o photon/photon.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) tailwag/tailwag.cpp -o tailwag/tailwag.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/to_stdout.cpp -o util/to_stdout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) -msse2 util/latency.cpp -o util/latency.o

clean:
	@rm -f *.o
	@rm -f util/*.o
	@rm -f pi/*.o
	@rm -f dropout/*.o
	@rm -f dop/*.o
	@rm -f swaptions/*.o
	@rm -f tailwag/*.o
	@rm -f photon/*.o
	@rm -f multinomial/*.o
	@rm -f *.ppm
