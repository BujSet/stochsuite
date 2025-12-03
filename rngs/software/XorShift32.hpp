#ifndef __XOR_SHIFT_32__
#define __XOR_SHIFT_32__

#include "RandomNumberGenerator.hpp"

#include <stdint.h>
#include <cstddef>

class XorShift32: public RNGBase {
    public:
        XorShift32();
        XorShift32(uint32_t seed);
        uint32_t _read_random() override;
        void _seed_random(uint32_t new_seed) override;
        std::string name() override;
};
#endif // __XOR_SHIFT_32__

