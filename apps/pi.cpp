#include "RNGFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include "Taus88.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    // std::string s("Taus88");
    // std::unique_ptr<RNGBase> rng = RNGFactory::createRNG(s);
    std::unique_ptr<Taus88> t88 = std::unique_ptr<Taus88>(new Taus88());
    if (!t88) {
        std::cout << "Failed to instantiate RNG instance!" << std::endl;
        return 1;
    }
    
	
	return 0;
}
