#ifndef __PARK_AND_MILLER_1988_GENERATOR__
#define __PARK_AND_MILLER_1988_GENERATOR__

#include "RandomNumberGenerator.hpp"

#include <stdint.h>
#include <cstddef>

class Park1988: public RNGBase {
    public:
        Park1988();
        Park1988(uint32_t seed);
        uint32_t _read_random() override;
        void _seed_random(uint32_t new_seed) override;
        std::string name() override;

        static const uint32_t a = 16807;
        static const uint32_t m = ;
        static const uint32_t q = ;
        static const uint32_t r = ;
    private:
        std::string _name;
};
#endif // __PARK_AND_MILLER_1988_GENERATOR__
