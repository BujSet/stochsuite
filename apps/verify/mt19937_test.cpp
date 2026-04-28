#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include "MersenneTwister.h"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>

int verify_state(const std::unique_ptr<RNGBase>& testRNG, MTRand *goldenRNG) {
    std::cout << "[INFO] Beginning state match verification...";
    for (size_t i = 0; i < 624; i++) {
        if (testRNG->dump_state_word(sizeof(uint32_t)*i) != goldenRNG->state[i]) {
            std::cout << std::endl << "ERROR: Incorrect state word from ";
            std::cout << "StochSuite implementation at state[";
            std::cout << i << "]=";
            std::cout << testRNG->dump_state_word(sizeof(uint32_t)*i);
            std::cout << ". Expected " << goldenRNG->state[i] << std::endl;
            return 1;
        }
    }
    std::cout << "Success!" << std::endl;
    return 0;
}

int main(int argc, char *argv[]) {

    size_t niters = 2000;
    uint32_t seed = 12312332;

    MTRand *randPtr = new MTRand(seed);
    std::cout << "[INFO] Created MTRand Object with seed " << seed << std::endl;

    std::string rngStr = "MersenneTwister";
    std::unique_ptr<RNGBase> rng = RNGFactory::createRNG(rngStr);
    if (!rng) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }
    std::cout << "[INFO] Initialized RNG: " << rng->name() << std::endl;

    rng->seed_random(seed);
    std::cout << "[INFO] Seeded both MTRand and StochSuite objects with " << seed << std::endl;
    if (verify_state(rng, randPtr)) {
        delete randPtr;
        return 1;
    }
    uint32_t golden_output, test_output;
    std::cout << "[INFO] Beginning output verification...";
    for (size_t i = 0; i < niters; i++) {
        golden_output = randPtr->randInt();
        test_output = rng->read_random();
        if (test_output != golden_output) {
            std::cout << std::endl << "ERROR: Incorrect output from ";
            std::cout << "StochSuite implementation at test num ";
            std::cout << i << ": ";
            std::cout << test_output;
            std::cout << ". Expected " << golden_output << std::endl;
            delete randPtr;
            return 1;
        }
    }
    std::cout << "Success!" << std::endl;

    if (verify_state(rng, randPtr)) {
        delete randPtr;
        return 1;
    }

    seed = 47281901;
    randPtr->seed(seed);
    rng->seed_random(seed);
    std::cout << "[INFO] Seeded both MTRand and StochSuite objects with " << seed << std::endl;
    if (verify_state(rng, randPtr)) {
        delete randPtr;
        return 1;
    }
    std::cout << "[INFO] Beginning output verification...";
    for (size_t i = 0; i < niters; i++) {
        golden_output = randPtr->randInt();
        test_output = rng->read_random();
        if (test_output != golden_output) {
            std::cout << std::endl << "ERROR: Incorrect output from ";
            std::cout << "StochSuite implementation at test num ";
            std::cout << i << ": ";
            std::cout << test_output;
            std::cout << ". Expected " << golden_output << std::endl;
            delete randPtr;
            return 1;
        }
    }
    std::cout << "Success!" << std::endl;

    if (verify_state(rng, randPtr)) {
        delete randPtr;
        return 1;
    }

    delete randPtr;
	return 0;
}
