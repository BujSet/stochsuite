include $(STOCHSUITE_HOME)/stochsuite.mk
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o) 

ifeq ($(UNAME), Linux)
    PLATFORM_CFLAGS += -msse2
endif

all:
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) pi.cpp -o pi.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/correctness.cpp -o util/correctness.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dropout.cpp -o dropout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) multinomial.cpp -o multinomial.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) dop.cpp -o dop.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) photon.cpp -o photon.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) tailwag.cpp -o tailwag.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/to_stdout.cpp -o util/to_stdout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) $(PLATFORM_CFLAGS) util/latency.cpp -o util/latency.o

clean:
	@rm -f *.o
	@rm -f util/*.o
	@rm -f *.ppm
