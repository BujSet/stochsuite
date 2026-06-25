include $(STOCHSUITE_HOME)/stochsuite.mk
RNG_OBJS := $(wildcard $(RNG_LIB)/*.o)

ifeq ($(OS), Linux)
    ifeq ($(ARCH), x86_64)
        PLATFORM_CFLAGS += -msse2
    endif
    ifeq ($(ARCH), aarch64)
        PLATFORM_CFLAGS += -march=armv8.5-a+rng
    endif
endif

# Keep only the stochastic-branch marker symbols in stripped binaries.
# objcopy --keep-symbol requires one flag per symbol name; since we use a
# consistent prefix we enumerate them via a helper variable.
STOCH_KEEP := \
	--keep-symbol=__stoch_br_pi \
	--keep-symbol=__stoch_br_dop \
	--keep-symbol=__stoch_br_dropout \
	--keep-symbol=__stoch_br_multinomial \
	--keep-symbol=__stoch_br_multinomial_wr \
	--keep-symbol=__stoch_br_photon_scatter \
	--keep-symbol=__stoch_br_photon_rr \
	--keep-symbol=__stoch_br_tailwag_0 \
	--keep-symbol=__stoch_br_tailwag_1 \
	--keep-symbol=__stoch_br_tailwag_2

STOCH_MEM_KEEP := \
	--keep-symbol=__stoch_mem_sgd_y \
	--keep-symbol=__stoch_mem_sgd_x

WORKLOADS := pi dropout multinomial dop photon tailwag

# FS build (default): address-mode m5 ops via mmap. Requires map_m5_mem() in
# the guest (needs /dev/gem5_bridge or /dev/mem). Works under KVM because the
# write to the m5ops MMIO range triggers KVM_EXIT_MMIO which gem5 intercepts.
all:
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) pi.cpp -o pi.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/correctness.cpp -o util/correctness.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) dropout.cpp -o dropout.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) multinomial.cpp -o multinomial.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) dop.cpp -o dop.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) photon.cpp -o photon.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_FS) tailwag.cpp -o tailwag.o
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) util/to_stdout.cpp -o util/to_stdout.o
	@if [ -f  /.dockerenv ]; then \
		echo "[WARN] Detected docker environment, skipping build of latency.o..."; \
	else \
	    $(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS) $(PLATFORM_CFLAGS) util/latency.cpp -o util/latency.o; \
	fi
	@for w in $(WORKLOADS); do \
		objcopy --strip-all $(STOCH_KEEP) $$w.o $$w.stripped; \
	done

# Full-system gem5: link libm5 and enable ROI markers (run `make gem5_fs` before
# populate_disk_image.sh).
gem5_fs:
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) pi.cpp $(M5OPS_LIBS) -o pi.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) dropout.cpp $(M5OPS_LIBS) -o dropout.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) multinomial.cpp $(M5OPS_LIBS) -o multinomial.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) dop.cpp $(M5OPS_LIBS) -o dop.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) photon.cpp $(M5OPS_LIBS) -o photon.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) tailwag.cpp $(M5OPS_LIBS) -o tailwag.o
	@$(CPP) -I$(RNG_LIB) -DGEM5_FS $(M5OPS_INC) $(RNG_OBJS) $(LFLAGS_FS) sgd/sgd.cpp sgd/functions.c $(M5OPS_LIBS) -o sgd/sgd.o
	@for w in $(WORKLOADS); do \
		objcopy --strip-all $(STOCH_KEEP) $$w.o $$w.stripped; \
	done
	@objcopy --strip-all $(STOCH_MEM_KEEP) sgd/sgd.o sgd/sgd.stripped

# SE build: no m5 ROI markers or libm5. Produces *_se binaries alongside FS *.o.
# -fno-if-conversion -fno-if-conversion2: prevent GCC from replacing stochastic
# branches (e.g. "if (z<=1.0) count++") with branchless comisd+sbb sequences.
# Without these flags the __stoch_br_* label is immediately followed by the
# loop-counter branch rather than the stochastic Jcc, causing StochBranchMonitor
# to track the wrong branch.
LFLAGS_SE := $(LFLAGS_FS) -fno-if-conversion -fno-if-conversion2
se:
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE pi.cpp -o pi_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE dropout.cpp -o dropout_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE multinomial.cpp -o multinomial_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE dop.cpp -o dop_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE photon.cpp -o photon_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE tailwag.cpp -o tailwag_se
	@$(CPP) -I$(RNG_LIB) $(RNG_OBJS) $(LFLAGS_SE) -DGEM5_SE_MODE sgd/sgd.cpp sgd/functions.c -o sgd/sgd_se
	@for w in $(WORKLOADS); do \
		objcopy --strip-all $(STOCH_KEEP) $${w}_se $${w}_se.stripped; \
	done
	@objcopy --strip-all $(STOCH_MEM_KEEP) sgd/sgd_se sgd/sgd_se.stripped

clean:
	@rm -f *.o *_se *.stripped *.ppm *.bin *.hex
	@rm -f sgd/sgd.o sgd/sgd_se sgd/sgd.stripped sgd/sgd_se.stripped
	@rm -f util/*.o
