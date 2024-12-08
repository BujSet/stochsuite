#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include "Taus88.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {

    size_t niters = 10;
    uint32_t seed = 1634404289;

    if (argc > 1) {
        if (argc > 6) {
            std::cout << "usage: ./" << argv[0] << " -seed <SEED> ";
            std::cout << " -iters <NUM_ITERS>" << std::endl;
            return 1;
        }
        for (size_t i = 0; i < (size_t)argc; i++) {
            if (strcmp("-seed", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                seed = (size_t) strtol(argv[i+1], NULL, 10);
                i++;
            }
            if (strcmp("-iters", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                niters = (size_t) strtol(argv[i+1], NULL, 10);
                i++;
            }

        }
    }
    std::unique_ptr<RNGBase> rng = std::unique_ptr<RNGBase>(new Taus88());
    if (!rng) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }

    rng->seed_random(seed);
    uint32_t rnd;
    for (size_t i = 0; i < niters; i++) {
        rnd = rng->read_random();
        std::cout << "Rand Value = " << rnd << std::endl;
    }
	return 0;
}
