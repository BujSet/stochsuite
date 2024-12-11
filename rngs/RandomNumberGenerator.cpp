#include "RandomNumberGenerator.hpp"
#include "RNGState.hpp"

#include <cmath>

RNGBase::RNGBase(size_t stateBytes)
    : state(new RNGState(stateBytes)),
      num_reseeds(0),
      num_rands_read(0) {
}

uint32_t RNGBase::read_random() {
    num_rands_read++;
    return _read_random();
}

double RNGBase::read_random_double() {
    uint32_t rand = _read_random();
    double frac = ((double) rand) / ((double) MAX());
    return frac;
}

// A simple implementation of the Box-Muller algorithm, used to generate
// gaussian random numbers - necessary for the Monte Carlo method below
// Note that C++11 actually provides std::normal_distribution<> in 
// the <random> library, which can be used instead of this function
double RNGBase::gaussian_box_muller() {
    double x = 0.0;
    double y = 0.0;
    double ret;
    double euclid_sq = 0.0;
    // Continue generating two uniform random variables
    // until the square of their "euclidean distance" 
    // is less than unity
    do {
        x = 2.0 * read_random_double() -1;
        y = 2.0 * read_random_double()-1;
        euclid_sq = x*x + y*y;
    } while (euclid_sq >= 1.0);
    ret = x*sqrt(-2*log(euclid_sq)/euclid_sq);
    return ret;
}

void RNGBase::seed_random(uint32_t new_seed) {
    num_reseeds++;
    _seed_random(new_seed);
}

RNGBase::~RNGBase() {
    delete state;
}
