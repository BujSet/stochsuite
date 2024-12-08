#ifndef __RANDOM_NUMBER_GENERATOR_BASE__
#define __RANDOM_NUMBER_GENERATOR_BASE__

#include "RNGState.hpp"

#include <stdint.h>

class RNGBase {
    public:
        RNGBase(size_t stateBytes);
        ~RNGBase();

        uint32_t read_random();
        virtual uint32_t _read_random() = 0;

        // Wrapper for calls to child class to allow for 
        // generalizable hooks and counters
        void seed_random(uint32_t new_seed);

        // Called by parent class to hook into child 
        // class implementations
        virtual void _seed_random(uint32_t new_seed) = 0;
    protected:
        RNGState* state;
    private:
        size_t num_reseeds;
        size_t num_rands_read;
};
#endif // __RANDOM_NUMBER_GENERATOR_BASE__
