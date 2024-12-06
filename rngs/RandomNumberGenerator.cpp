#include "RandomNumberGenerator.hpp"
#include "RNGState.hpp"

RNGBase::RNGBase(size_t stateBytes)
    : state(new RNGState(stateBytes)){
}

RNGBase::~RNGBase() {}
