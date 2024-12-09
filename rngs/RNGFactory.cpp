#include "RNGFactory.hpp"

#include "Taus88.hpp"
#include "Taus113.hpp"
#include "JKISS.hpp"
#include "JKISS32.hpp"
#include "LCG.hpp"

#include <string>
#include <memory>

RNGFactory::RNGFactory() {}

RNGFactory::~RNGFactory() {}

std::unique_ptr<RNGBase> RNGFactory::createRNG(const std::string type) {
    if (type == "Taus88") {
        return std::unique_ptr<RNGBase>(new Taus88());
    } else if (type == "Taus113") {
        return std::unique_ptr<RNGBase>(new Taus113());
    } else if (type == "JKISS") {
        return std::unique_ptr<RNGBase>(new JKISS());
    } else if (type == "JKISS32") {
        return std::unique_ptr<RNGBase>(new JKISS32());
    } else if (type == "LCG_CONG") {
        // Based on KISS paper
        // https://eprint.iacr.org/2011/007.pdf
        return std::unique_ptr<RNGBase>(new LCG(123132, 69069, 1234567, 0xFFFFFFFF, "LCG_CONG"));
    } else if (type == "LCG_GLIBC_CRAND") {
        // Based on C implementation of random_r.c
        // https://sourceware.org/git/?p=glibc.git;a=blob;f=stdlib/random_r.c;hb=glibc-2.28#l353
        return std::unique_ptr<RNGBase>(new LCG(123132, 1103515245U, 12345U, 0x7FFFFFFF, "LCG_GLIBC_C_RAND"));
    } else if (type == "LCG_DRAND48") {
        // Based on C implementation of drand48-iter.c
        // https://codebrowser.dev/glibc/glibc/stdlib/drand48-iter.c.html#__drand48_iterate
        return std::unique_ptr<RNGBase>(new LCG(1, 0x5deece66dull, 0xb, 0xFFFFFFFF, "LCG_DRAND48_C_RAND"));
    } else {
        return nullptr;
    }
}
