#include "Taus88.hpp"

Taus88::Taus88() : RNGBase(12) {
    C1 = 0xFFFFFFFE;
    C2 = 0xFFFFFFF8;
    C3 = 0xFFFFFFF0;
    state->set_state_bytes_from_int(1634404289, 4, 0);
    state->set_state_bytes_from_int(8, 4, 4);
    state->set_state_bytes_from_int(16, 4, 8);
}

Taus88::Taus88(uint32_t S1, uint32_t S2, uint32_t S3) : RNGBase(12) {
    C1 = 0xFFFFFFFE;
    C2 = 0xFFFFFFF8;
    C3 = 0xFFFFFFF0;
    state->set_state_bytes_from_int(S1, 4, 0);
    state->set_state_bytes_from_int(S2, 4, 4);
    state->set_state_bytes_from_int(S3, 4, 8);
}

void Taus88::_seed_random(uint32_t new_seed) {
    state->set_state_bytes_from_int(new_seed, 4, 0);
}

std::string Taus88::name() {
    return "Taus88";
}

uint32_t Taus88::MAX() {
    return 0xFFFFFFFF;
}

uint32_t Taus88::MIN() {
    return 0;
}

uint32_t Taus88::_read_random(){
    uint32_t S1 = state->get_state_bytes_as_int(0);
    uint32_t S2 = state->get_state_bytes_as_int(4);
    uint32_t S3 = state->get_state_bytes_as_int(8);
    S1 = ( ( S1 & C1 ) << 12 ) ^ ( ( ( S1 << 13 ) ^ S1 ) >> 19);
    S2 = ( ( S2 & C2 ) <<  4 ) ^ ( ( ( S2 <<  2 ) ^ S2 ) >> 25);
    S3 = ( ( S3 & C3 ) << 17 ) ^ ( ( ( S3 <<  3 ) ^ S3 ) >> 11);
    state->set_state_bytes_from_int(S1, 4, 0);
    state->set_state_bytes_from_int(S2, 4, 4);
    state->set_state_bytes_from_int(S3, 4, 8);
    return (S1 ^ S2 ^ S3);
}
