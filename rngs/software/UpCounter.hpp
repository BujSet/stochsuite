#ifndef __UP_COUNTER_GENERATOR__
#define __UP_COUNTER_GENERATOR__

#include "RandomNumberGenerator.hpp"

#include <stdint.h>
#include <cstddef>

class UpCounter: public RNGBase {
    public:
        UpCounter();
        UpCounter(uint32_t seed);
        uint32_t _read_random() override;
        void _seed_random(uint32_t new_seed) override;
        std::string name() override;
};
#endif // __UP_COUNTER_GENERATOR__
