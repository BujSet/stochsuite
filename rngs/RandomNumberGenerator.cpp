#include "RandomNumberGenerator.hpp"
#include "RNGState.hpp"

RNGBase::RNGBase(size_t stateBytes)
    : state(new RNGState(stateBytes)),
      num_reseeds(0),
      num_rands_read(0) {
}

uint32_t RNGBase::read_random() {
    num_rands_read++;
    return _read_random();
}

void RNGBase::seed_random(uint32_t new_seed) {
    num_reseeds++;
    _seed_random(new_seed);
}

RNGBase::~RNGBase() {}
