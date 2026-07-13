#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]) {
    std::string rngStr = "MersenneTwister";
    uint32_t seed = 12312332;

    if (argc > 1) {
        if (argc > 7) {
            std::cout << "usage: ./" << argv[0] << " -seed <SEED> ";
            std::cout << " -rng <RNG>" << std::endl;
            return 1;
        }
        for (size_t i = 0; i < (size_t)argc; i++) {
            if (strcmp("-seed", argv[i]) == 0) {
                assert(i + 1 < (size_t)argc);
                seed = (size_t) strtol(argv[i+1], NULL, 10);
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
    

    std::ofstream outfile(rngStr + "_seed_" + std::to_string(seed) + ".memh");
    for(int i = 0; i < 624; ++i) {
        // Write each integer in hex without the '0x' prefix
        outfile << std::hex << rng->dump_state_word(4*i) << "\n"; 
    }
    outfile.close();

    for (int i = 0; i < 625; ++i) {
        rng->read_random();
    }

    std::ofstream outfile_reload(rngStr + "_seed_" + std::to_string(seed) + "_reload_1.memh");
    for(int i = 0; i < 624; ++i) {
        // Write each integer in hex without the '0x' prefix
        outfile_reload << std::hex << rng->dump_state_word(4*i) << "\n"; 
    }
    outfile_reload.close();

	return 0;
}
