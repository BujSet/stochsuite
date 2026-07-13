#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
    size_t niters = 100000000000;
    size_t arr_size = 100000;
    uint32_t seed = 1634404289;
    std::string rngStr = "Taus88";

    if (argc > 1) {
        if (argc > 9) {
            std::cout << "usage: ./" << argv[0] << " -seed <SEED> ";
            std::cout << " -iters <NUM_ITERS> -rng <RNG>" ;
            std::cout << " -arr <ARR_SIZE>" << std::endl;
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
            if (strcmp("-arr", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                arr_size = (size_t) strtol(argv[i+1], NULL, 10);
                i++;
            }
        }
    }

    float *arr = new float[arr_size];
    for (size_t i = 0; i < arr_size; i++) {
        arr[i] = ((float) i) + 0.5f;
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
        rnd = rng->read_random_range(0, arr_size);
        __asm__ volatile ("stochastic_memory_access:");
        sum += arr[rnd];
    }
    // ROI end

    // Use output to prevent compiler from optimizing away the loop
    std::cout << "Sum: " << sum << std::endl;
    delete[] arr;
	return 0;
}
