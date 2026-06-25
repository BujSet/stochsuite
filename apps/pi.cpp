#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#ifdef GEM5_FS
#include "m5_roi.h"
#endif

int main(int argc, char *argv[]) {

    size_t niters = 100;
    uint32_t seed = 12312332;
    std::string rngStr = "Taus88";

    if (argc > 1) {
        if (argc > 7) {
            std::cout << "usage: ./" << argv[0] << " -seed <SEED> ";
            std::cout << " -iters <NUM_ITERS> -rng <RNG>" << std::endl;
            return 1;
        }
        for (size_t i = 0; i < (size_t)argc; i++) {
            if (strcmp("-seed", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                seed = (uint32_t) strtol(argv[i+1], NULL, 10);
                i++;
            }
            if (strcmp("-iters", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                niters = (size_t) strtol(argv[i+1], NULL, 10);
                i++;
            }
            if (strcmp("-rng", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                rngStr = (std::string) argv[i+1];
                i++;
            }
        }
    }
    double x, y, z, pi;
    size_t count = 0;

    std::unique_ptr<RNGBase> rng = RNGFactory::createRNG(rngStr);
    if (!rng) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }
    std::cout << "Successfully initialized RNG: " << rng->name() << std::endl;

    rng->seed_random(seed);
#ifdef GEM5_FS
    map_m5_mem();
    M5_ROI_BEGIN();
#endif
    for (size_t i = 0; i < niters; i++) {
        x = rng->read_random_double();
        y = rng->read_random_double();
        z = sqrt((x*x) + (y*y));
        asm volatile(".globl __stoch_br_pi\n__stoch_br_pi:");
        if (z <= 1.0) {
            count++;
        }
    }
    pi = 4.0 * ((double)count) / ((double)niters);
#ifdef GEM5_FS
    M5_ROI_END();
#endif

    std::cout << "pi = " << pi << std::endl;
	return 0;
}
