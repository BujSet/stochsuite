#include "RandomNumberGenerator.hpp"

RNGBase::RNGBase(size_t stateBytes) {
    state = new RNGState(stateBytes);
}

RNGBase::~RNGBase() {
    delete state;
}
