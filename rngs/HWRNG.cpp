#include "HWRNG.hpp"

#include <stdexcept>

#if defined(__x86_64__)
    #include <immintrin.h>
#endif

#if defined(__aarch64__)
    #include <arm_acle.h>
#endif


HWRNG::HWRNG() : RNGBase(0) {
#if defined(__x86_64__)
    _name = "X86_64_RDRAND";
#elif defined(__aarch64__)

#if __ARM_FEATURE_RNG
    _name = "AARCH64_RDRAND";
#else
    throw std::invalid_argument("Arm processor does not support HWRNG");
#endif

#elif defined(__i386__)
    throw std::invalid_argument("i386 does not support HWRNGs");
#else 
    throw std::invalid_argument("Unsupported architecture");
#endif
}

void HWRNG::_seed_random(uint32_t new_seed) {
    return;
}

std::string HWRNG::name() {
    return _name;
}

#if defined(__x86_64__)
// adapted from https://software.intel.com/en-us/articles/intel-digital-random-number-generator-drng-software-implementation-guide
uint32_t _x86_64_rdrand_step(uint32_t *rand) {
    unsigned char ok;

    __asm__ volatile("rdrand %0;"
                     "setc %1"
                     : "=r"(*rand), "=qm"(ok));

    return (int)ok;
}
#endif

uint32_t HWRNG::_read_random() {
    uint32_t rndval = 0;
#if defined(__x86_64__)
    while(_x86_64_rdrand_step(&rndval) == 0);
    return rndval;
#elif defined(__aarch64__)
    // Based on https://developer.arm.com/documentation/101028/0012/8--Data-processing-intrinsics
    uint64_t rand;
    while(_rndr(&rand) != 0);
    rndval = rand & 0xFFFFFFFF;
    return rndval;
#else
    return rndval;
#endif
}
