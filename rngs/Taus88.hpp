#ifndef __TAUSWORTHE_88__
#define __TAUSWORTHE_88__

#include "RandomNumberGenerator.hpp"

class Taus88: public RNGBase {
    public:
        Taus88();
        Taus88(uint32_t S1, uint32_t S2, uint32_t S3);
        uint32_t read_random() override;
};
#endif // __TAUSWORTHE_88__

