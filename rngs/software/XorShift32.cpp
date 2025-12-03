#include "XorShift32.hpp"

#include <cassert>

XorShift32::XorShift32() : RNGBase(4) {
    state->set_state_bytes_from_int(2463534242, 4, 0);
}

XorShift32::XorShift32(uint32_t seed) : RNGBase(4) {
    // Seeding rules prohibit a seed of 0
    assert(seed != 0);
    state->set_state_bytes_from_int(seed, 4, 0);
}

void XorShift32::_seed_random(uint32_t new_seed) {
    // Seeding rules prohibit a seed of 0
    assert(new_seed != 0);
    state->set_state_bytes_from_int(new_seed, 4, 0);
}

std::string XorShift32::name() {
    return "XorShift32";
}

uint32_t XorShift32::_read_random(){
    uint32_t s = state->get_state_bytes_as_int(0);
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s <<  5;
    state->set_state_bytes_from_int(s, 4, 0);
    return s;
}
