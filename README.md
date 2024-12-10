# TODO

## Random Number Generator Suite

* TODO: implement other RNGS:
    * Yarrow: https://www.schneier.com/academic/yarrow/
    * Fortuna: https://www.schneier.com/wp-content/uploads/2015/12/fortuna.pdf
    * PCGBasic: Seems to be broken... Need to fix
    * Parallel Random Number Generation: https://www.thesalmons.org/john/random123/papers/random123sc11.pdf
    * HWRNG: should add check for x86_64 support for DRNG for graceful error message rather than program crash 