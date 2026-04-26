#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include "MersenneTwister.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

int main(int argc, char *argv[]) {
    MTRand *randPtr = new MTRand();
    std::cout << "[INFO] Created MTRand Object" << std::endl;

    size_t niters = 600;
    uint32_t seed = 12312332;
    std::string rngStr = "MersenneTwister";
    std::unique_ptr<RNGBase> rng = RNGFactory::createRNG(rngStr);
    if (!rng) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }
    std::cout << "[INFO] Initialized RNG: " << rng->name() << std::endl;

    randPtr->seed(seed);
    rng->seed_random(seed);
    std::cout << "[INFO] Seeded both MTRand and StochSuite objects" << std::endl;

    std::cout << "Offset: StochSuite State Word ? MTRand State Word" << std::endl;
    std::cout << 0 << ": " << rng->dump_state_word(0) << " ? " << randPtr->state[0] << std::endl;
    std::cout << 1 << ": " << rng->dump_state_word(1) << " ? " << randPtr->state[1] << std::endl;


    std::cout << "[INFO] MTRand rand output: " << randPtr->randInt() << std::endl;

    std::cout << "[INFO] StochSuite rand output: " << rng->read_random() << std::endl;
    delete randPtr;
	return 0;
}
