#include "Park1988.hpp"

Park1988::Park1988() : RNGBase(4) {
    state->set_state_bytes_from_int(1979, 4, 0);
}

Park1988::Park1988(uint32_t seed): RNGBase(4) {
    _name = name;
    state->set_state_bytes_from_int(seed, 4, 0);
}

void Park1988::_seed_random(uint32_t new_seed) {
    state->set_state_bytes_from_int(new_seed, 4, 0);
}


std::string Park1988::name() {
    return "Park1988";
}

uint32_t LCG::_read_random(){
    uint32_t s = state->get_state_bytes_as_int(0);
    uint32_t hi, lo;
    int32_t test;
    hi = s / Park1988::q;
    lo = s % Park1988::q;
    test = (Park1988::a * lo) - (Park1988::r * hi);
    s = test;
    if (test <= 0) {
        s += Park1988::m;
    }
    state->set_state_bytes_from_int(s, 4, 0);
    return s / Park1988::m;
}
