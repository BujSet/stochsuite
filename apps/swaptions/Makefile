TARGET = swaptions

ifdef version
  ifeq "$(version)" "pthreads" 
    DEF := $(DEF) -DENABLE_THREADS
    CXXFLAGS := $(CXXFLAGS) -pthread
  endif
  ifeq "$(version)" "tbb"
    DEF := $(DEF) -DENABLE_THREADS -DTBB_VERSION
    LIBS := $(LIBS) -ltbb
  endif
endif


all: $(TARGET)

$(TARGET): 
	g++ $(SNIPER_CXXFLAGS) $(DEF) icdf.cpp $(INCLUDE) $(LIBS) -c -o icdf.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) nr_routines.c $(INCLUDE) $(LIBS) -c -o nr_routines.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) MaxFunction.cpp $(INCLUDE) $(LIBS) -c -o MaxFunction.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) RanUnif.cpp $(INCLUDE) $(LIBS) -c -o RanUnif.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) HJM_Swaption_Blocking.cpp $(INCLUDE) $(LIBS) -c -o HJM_Swaption_Blocking.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) HJM_SimPath_Forward_Blocking.cpp $(INCLUDE) $(LIBS) -c -o HJM_SimPath_Forward_Blocking.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) HJM.cpp $(INCLUDE) $(LIBS) -c -o HJM.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) CumNormalInv.cpp $(INCLUDE) $(LIBS) -c -o CumNormalInv.o
	g++ $(SNIPER_CXXFLAGS) $(DEF) HJM_Securities.cpp $(INCLUDE) $(LIBS) -c -o HJM_Securities.o
	g++ MaxFunction.o nr_routines.o icdf.o HJM.o HJM_SimPath_Forward_Blocking.o HJM_Securities.o CumNormalInv.o HJM_Swaption_Blocking.o RanUnif.o -lm -static -pthread -o $(TARGET)

asm: 
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) HJM.cpp $(INCLUDE) $(LIBS) -S -o HJM.s
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) CumNormalInv.cpp $(INCLUDE) $(LIBS) -S -o CumNormalInv.s
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) HJM_SimPath_Forward_Blocking.cpp $(INCLUDE) $(LIBS) -S -o HJM_SimPath_Forward_Blocking.s

accuracy:
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) RanUnif.cpp $(INCLUDE) $(LIBS) -c -o RanUnif.o
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) CumNormalInv.cpp $(INCLUDE) $(LIBS) -c -o CumNormalInv.o 
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) HJM_Securities.cpp $(INCLUDE) $(LIBS) -c -o HJM_Securities.o 
	$(SNIPER_LD)  HJM_Securities.o CumNormalInv.o -lm $(SNIPER_LDFLAGS) -o $(TARGET) 
	echo "Running normal execution ..."
	./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) RanUnif.cpp $(INCLUDE) $(LIBS) -c -o RanUnif.o -DPBS_HOOKS=1
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) CumNormalInv.cpp $(INCLUDE) $(LIBS) -c -o CumNormalInv.o -DPBS_HOOKS=1
	$(CPP) $(SNIPER_CXXFLAGS) $(DEF) HJM_Securities.cpp $(INCLUDE) $(LIBS) -c -o HJM_Securities.o -DPBS_HOOKS=1
	$(SNIPER_LD) HJM_Securities.o CumNormalInv.o -lm $(SNIPER_LDFLAGS) -o $(TARGET) -DPBS_HOOKS=1
	echo "Running PBS execution ..."
	./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 

daniel8K : $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='daniel8K' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/daniel8K.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

collect: 
	python3 ../../scripts/collect_stats.py

pentium128 : $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium128' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium128.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

pentium1K : $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium1K' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium1K.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

pentium8K: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium_m' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium8K.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tournament1K: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tournament' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tournament1K.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tournament1K_perf_pb: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tournament_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tournament1K_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl8K: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl' -g --perf_model/core/type="interval"  --roi -s markers:stats -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	cp sim.out data/tagescl8K.out
#rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl64K: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl64K' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl64K.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

pentium128_perf_pb : $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium128_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium128_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

pentium1K_perf_pb : $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium1K_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium1K_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

pentium8K_perf_pb: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='pentium_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/pentium8K_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl8K_perf_pb: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl8K_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl64K_perf_pb: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl64K_perf_pb' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl64K_perf_pb.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl8K_perf_pma: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl' -g --perf_model/core/type="pma"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl8K_perf_pma.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl8K_perf_rfc: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl' -g --perf_model/core/type="rfc"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl8K_perf_rfc.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

tagescl8K_perf_all: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='tagescl_perf_pb' -g --perf_model/core/type="pma_rfc"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/tagescl8K_perf_all.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

one: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='one_bit' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/one.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

two: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='two_bit_sat' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/two.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

perfect: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='perfect' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/perfect.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

nt: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='nt' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/nt.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

none: $(TARGET)
	../../run-sniper -v -n 1 -c sandy_bridge -g --perf_model/branch_predictor/type='none' -g --perf_model/core/type="interval"  --roi -- ./$(TARGET) -ns 16 -sm 5000 -nt 1 -sd 31 #Simsmall params = 16,5000
	mv sim.out data/none.out
	rm -f sim.stats.sqlite3 sim.cfg sim.info

data: perfect pentium128 pentium128_perf_pb pentium1K pentium1K_perf_pb pentium8K pentium8K_perf_pb tagescl8K tagescl8K_perf_pb tagescl64K tagescl64K_perf_pb daniel8K

min_data: pentium128 pentium128_perf_pb tagescl8K tagescl8K_perf_pb 

extras: pentium8K pentium8K_perf_pb tagescl8K tagescl8K_perf_pb tagescl64K tagescl64K_perf_pb

tage8: tagescl8K tagescl8K_perf_pb tagescl8K_perf_pma tagescl8K_perf_rfc tagescl8K_perf_all

clean: 
	rm -f *.o
	rm -f swaptions

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEF) -c $*.cpp -o $*.o

.c.o:
	$(CXX) $(CXXFLAGS) $(DEF) -c $*.c -o $*.o

