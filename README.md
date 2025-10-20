# Environment Setup

## Requirements

TODO: Docker

## Building Docker Image Locally

One option to develop and run the code in this repo is to build a docker image 
locally. You can inspect and modify the provided [Dockerfile](Dockerfile) to 
add any additional packages you may need.

To build an image, rung the following in the same directory as the:

```
docker build -t stochsuite:v4_ubuntu_22.04 .
```

## Pulling Exisitng Image from DockerHub

If you do not need any additional packages, you can pull the image directly from 
dockerhub via:

```
docker pull rselagam/stochsuite:v4_ubuntu_22.04
```

## Starting an Interactive Container

Once the image is either built of pulled down to your local machine, it is 
ready to be used for running a container. E.g. you can do this via:

```
docker run -it --rm -w /root -e "STOCHSUITE_HOME=/root/stochsuite/"  rselagam/stochsuite:v4_ubuntu_22.04 /bin/bash -c "git clone https://github.com/BujSet/stochsuite.git && cd stochsuite/ && /bin/bash"
```

The command above creates a docker container with the necessary environment 
vairables to use the Makefiles in the repo. It also clones the repo to the 
correct path location to match the environment variable before yielding control 
back to the user.

# Building and Running RNG Applications

## Building RNG Library

The generator library must be built before the applications. Run the following

```
cd $STOCHSUITE_HOME/rngs/
make
```

The command above will compile all the C++ implementations of the random number 
generators. 

## Building the Application Library

After building the RNG library, we can now build the applications:

```
cd $STOCHSUITE_HOME/apps/
make
```

This will generate object files that can be run in the `apps` directory.

# Running an example application 

## The Correctness Application

The command below will run the correctness application for the JKISS generator. 
See the RNGFactory.cpp file for a list of generator options.

```
./correctness.o -iters 5 -seed 0xdeadbeef -rng JKISS
```

You should see the following output:

```
Successfully initialized RNG: JKISS
Rand Value = 1557129256
Rand Value = 3607227500
Rand Value = 2958840619
Rand Value = 3983503383
Rand Value = 1414710373
```

# TODO

## Random Number Generator Suite

* TODO: implement other RNGS:
    * Yarrow: https://www.schneier.com/academic/yarrow/
    * Fortuna: https://www.schneier.com/wp-content/uploads/2015/12/fortuna.pdf
    * PCGBasic: Seems to be broken... Need to fix
    * Parallel Random Number Generation: https://www.thesalmons.org/john/random123/papers/random123sc11.pdf
    * HWRNG: should add check for x86_64 support for DRNG for graceful error message rather than program crash
    * Middle-squared method: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/generating-random-numbers.html

## Applications:

* TODO: add more apps
    * Monte Carlo McBeth Color Chart Approximator via Integration Approximation: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/monte-carlo-rendering-practical-example.html, https://github.com/scratchapixel/scratchapixel-code/blob/main/monte-carlo-methods-in-practice/mcintegration.cpp, https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/monte-carlo-integration.html
