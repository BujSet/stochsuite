#include "RNGState.hpp"

#include <cassert>

RNGState::RNGState(size_t numBytes)
    : numBytes(numBytes) {
    allState = new uint8_t[numBytes];
}

uint8_t *RNGState::getState() {
    return allState;
}

void RNGState::set_state_bytes_from_int(uint32_t new_state, 
            uint32_t valid_bytes, size_t state_offset) {
    if (valid_bytes == 0) return;
    assert(valid_bytes <= 4);
    assert(state_offset + valid_bytes <= numBytes);
    for (size_t i = 0; i < valid_bytes; i++) {
        allState[state_offset + i] = (uint8_t)((new_state >> (8*i)) & 0xFF);
    }
}

uint32_t RNGState::get_state_bytes_as_int(size_t state_offset) {
    assert(state_offset < numBytes);
    assert(state_offset + 4 <= numBytes);
    uint32_t result = 0;
    for (size_t i = 0; i < 4; i++) {
        result |= allState[i + state_offset] << (8*i);
    }
    return result;
}

size_t RNGState::size() { 
    return numBytes; 
} 

RNGState::~RNGState() {
    delete[] allState;
}
