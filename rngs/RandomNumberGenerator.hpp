#ifndef __RANDOM_NUMBER_GENERATOR_BASE__
#define __RANDOM_NUMBER_GENERATOR_BASE__

#include "RNGState.hpp"

class RNGBase {
    public:
        RNGBase(size_t stateBytes);
        ~RNGBase();
        virtual uint32_t read_random() = 0;
    protected:
        RNGState& state;
};
#endif // __RANDOM_NUMBER_GENERATOR_BASE__
