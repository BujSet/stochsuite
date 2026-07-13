#include "UpCounter.hpp"

UpCounter::UpCounter() : RNGBase(4) {
    state->set_state_bytes_from_int(0, 4, 0);
}

UpCounter::UpCounter(uint32_t seed): RNGBase(4) {
    state->set_state_bytes_from_int(seed, 4, 0);
}

void UpCounter::_seed_random(uint32_t new_seed) {
    state->set_state_bytes_from_int(new_seed, 4, 0);
}

std::string UpCounter::name() {
    return "UpCounter";
}

uint32_t UpCounter::_read_random(){
    uint32_t s = state->get_state_bytes_as_int(0);
    state->set_state_bytes_from_int(++s, 4, 0);
    return s;
}
