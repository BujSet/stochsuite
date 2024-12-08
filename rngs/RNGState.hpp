#ifndef __RANDOM_NUMBER_GENERATOR_STATE__
#define __RANDOM_NUMBER_GENERATOR_STATE__

#include <stdint.h>
#include <cstddef>

class RNGState {
    public:
        RNGState(size_t numBytes);
        ~RNGState();
        void set_state_bytes_from_int(uint32_t new_state, 
            uint32_t valid_bytes, size_t state_offset);
        uint32_t get_state_bytes_as_int(size_t state_offset);
        uint8_t *getState();
        size_t size();
    protected:
        uint8_t *allState;
        size_t numBytes;
};
#endif // __RANDOM_NUMBER_GENERATOR_STATE__
