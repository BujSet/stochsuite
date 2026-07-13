#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
    size_t niters = 100000000000;
    uint32_t seed = 1634404289;
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
                seed = (size_t) strtol(argv[i+1], NULL, 10);
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
    
    std::unique_ptr<RNGBase> rng = RNGFactory::createRNG(rngStr);
    if (!rng) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }

    rng->seed_random(seed);
    double sum = 0.0;
    uint32_t rnd;

    // ROI begin
    for (size_t i = 0; i < niters; i++) {
        rnd = rng->read_random();
        __asm__ volatile ("stochastic_branch_0:");
        if (rnd % 10 == 0) {
            sum += 0.1;
        } 
        __asm__ volatile ("stochastic_branch_1:");
        if (rnd % 10 == 1) {
            sum += 0.2;
        }
        __asm__ volatile ("stochastic_branch_2:");
        if (rnd % 10 == 2) {
            sum += 0.3;
        }
        __asm__ volatile ("stochastic_branch_3:");
        if (rnd % 10 == 3) {
            sum += 0.4;
        }
        __asm__ volatile ("stochastic_branch_4:");
        if (rnd % 10 == 4) {
            sum += 0.5;
        } 
        __asm__ volatile ("stochastic_branch_5:");
        if (rnd % 10 == 5) {
            sum += 0.6;
        }
        __asm__ volatile ("stochastic_branch_6:");
        if (rnd % 10 == 6) {
            sum += 0.7;
        }
        __asm__ volatile ("stochastic_branch_7:");
        if (rnd % 10 == 7) {
            sum += 0.8;
        }
        __asm__ volatile ("stochastic_branch_8:");
        if (rnd % 10 == 8) {
            sum += 0.9;
        } 
        __asm__ volatile ("stochastic_branch_9:");
        if (rnd % 10 == 9) {
            sum += 1.0;
        }
    }
    // ROI end
    
    // Use output to prevent compiler from optimizing away the loop
    std::cout << "Sum: " << sum << std::endl;
	return 0;
}
