#include "Taus88.hpp"

Taus88::Taus88() : RNGBase(12) {
    // Set state
}

Taus88::Taus88(uint32_t S1, uint32_t S2, uint32_t S3) : RNGBase(12) {
    // Set state
}

uint32_t Taus88::read_random(){
    return 0;
}
