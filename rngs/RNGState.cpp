#include "RNGState.hpp"

RNGState::RNGState(size_t numBytes)
    : numBytes(numBytes) {
    allState = new uint8_t[numBytes];
}

uint8_t *RNGState::getState() {
    return allState;
}

size_t RNGState::size() { 
    return numBytes; 
} 

RNGState::~RNGState() {
    delete[] allState;
}
