#include "RNGState.hpp"

RNGState::RNGState(size_t numBytes)
    : size(numBytes) {
    allState = new uint8_t[numBytes];
}

uint8_t *RNGState::getState() {
    return allState;
}

size_t RNGState::size() { 
    return size; 
} 

void RNGState::~RNGState() {
    delete[] allState;
}
