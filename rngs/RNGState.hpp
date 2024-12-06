#ifndef __RANDOM_NUMBER_GENERATOR_STATE__
#define __RANDOM_NUMBER_GENERATOR_STATE__

class RNGState {
    public:
        RNGState(size_t numBytes);
        ~RNGState();
        uint8_t *getState();
        size_t size();
    protected:
        uint8_t *allState;
        size_t size;
};
#endif // __RANDOM_NUMBER_GENERATOR_STATE__
