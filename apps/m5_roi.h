#ifndef M5_ROI_H
#define M5_ROI_H

#ifdef GEM5_FS
#include <gem5/m5ops.h>
#include "m5_mmap.h"

// NOP spin counts sized to exceed simQuantum (1 ms of simulated time).
// GEM5_SPIN_KVM: spins on KVM (~3 GHz native); 10 M NOPs ≈ 3 ms.
// GEM5_SPIN_O3:  spins on O3 (1 GHz simulated clock, IPC≈3); 2 M NOPs ≈ 2 ms.
#define GEM5_SPIN_KVM 10000000
#define GEM5_SPIN_O3   2000000

static inline void gem5_nop_spin(int n) {
    for (volatile int i = 0; i < n; i++)
        asm volatile("nop");
}

#define M5_ROI_BEGIN() do { \
    m5_hypercall_addr(10); \
    gem5_nop_spin(GEM5_SPIN_KVM); \
    m5_work_begin_addr(0, 0); \
} while (0)

#define M5_ROI_END() do { \
    m5_work_end_addr(0, 0); \
    m5_hypercall_addr(11); \
    gem5_nop_spin(GEM5_SPIN_O3); \
} while (0)

#endif /* GEM5_FS */

#endif /* M5_ROI_H */
