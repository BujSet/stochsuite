# Environment Setup

## Requirements

Docker is a platform that uses operating-system-level virtualization to
deliver software in packages called containers. This generators and
applications in this suite have been functionally verified on the
following versions:

| Application | Version | Notes |
|---|---|---|
| Docker Engine| 27.3.1 | build ce12230 |
| Docker Desktop | 4.35.0 | (172550) |
| Docker Compose | v2.29.7-desktop.1 | |

## Building Docker Image Locally

One option to develop and run the code in this repo is to build a docker image
locally. You can inspect and modify the provided [Dockerfile](Dockerfile) to
add any additional packages you may need.

To build an image, rung the following in the same directory as the:

```
docker buildx create --use --name multi-platform-builder
docker buildx inspect multi-platform-builder --bootstrap
docker buildx build --platform linux/arm64,linux/amd64 \
  -t rselagam/stochsuite:v2_ubuntu_24.04 \
  --push .
```

## Pulling Exisitng Image from DockerHub

If you do not need any additional packages, you can pull the image directly from
dockerhub via:

```
docker pull rselagam/stochsuite:v2_ubuntu_24.04
```

## Starting an Interactive Container

Once the image is either built of pulled down to your local machine, it is
ready to be used for running a container. E.g. you can do this via:

```
docker run -it --rm -w /root -e "STOCHSUITE_HOME=/root/stochsuite"  rselagam/stochsuite:v2_ubuntu_24.04 /bin/bash -c "git clone https://github.com/BujSet/stochsuite.git && cd stochsuite/ && /bin/bash"
```

The command above creates a docker container with the necessary environment
vairables to use the Makefiles in the repo. It also clones the repo to the
correct path location to match the environment variable before yielding control
back to the user. After entering the project directory, run:

```
git submodule update --init --recursive
```

To ensure that all submodules (e.g., gem5, Xilinx lib, etc.) are pulled and available
to use.

# Building and Running RNG Applications

## Building RNG Library

The generator library must be built before the applications. Run the following

```
cd $STOCHSUITE_HOME/rngs/software/
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

Running the command below from the `apps/` directory will run the correctness
application for the JKISS generator. See the [RNGFactory.cpp](rngs/software/RNGFactory.cpp)
file for a list of generator options.

```
./util/correctness.o -iters 5 -seed 0xdeadbeef -rng JKISS
```

You should see the following output:

```
Successfully initialized RNG: JKISS
Rand Value = 2778845915
Rand Value = 2959504851
Rand Value = 54303350
Rand Value = 4063145910
Rand Value = 255296776
```

# gem5 Simulation

Stochsuite workloads can be studied in gem5 in two ways:

- **Syscall emulation (SE)** — runs `apps/<benchmark>_se` directly on an O3
  CPU (no OS boot, no disk image). A `StochBranchMonitor` on the branch
  predictor records per-stochastic-branch predict/mispredict stats in
  `stats.txt`ccalled stoch_branch_monitor. Config: `$GEM5_HOME/configs/x86-se-pi.py`.
- **Full system (FS)** — boots Ubuntu on KVM, runs `*_gem5` binaries from
  the disk image inside an ROI bounded by `m5_work_begin` / `m5_work_end`,
  then dumps ROI stats.  A `StochBranchMonitor` on the branch
  predictor records per-stochastic-branch predict/mispredict stats in
  `stats.txt`ccalled stoch_branch_monitor. Config:
  `$GEM5_HOME/configs/x86-fs-stochsuite-workloads.py`.

## One-time setup

The Makefiles all include `$(STOCHSUITE_HOME)/stochsuite.mk`. **Export
`STOCHSUITE_HOME` before any `make` invocation** (the Docker workflow
above sets it automatically; outside Docker you set it yourself). FS
builds also need `GEM5_HOME` so the Makefile can link `libm5`:

```
export STOCHSUITE_HOME=/absolute/path/to/stochsuite
export GEM5_HOME=/absolute/path/to/gem5
```

For this checkout that is:

```
export GEM5_HOME=$HOME/work/stochsuite_gem5/gem5
export STOCHSUITE_HOME=$HOME/work/stochsuite_gem5/stochsuite
```

1. Build `libm5` for the gem5 utility (the workloads link against this):

   ```
   cd $GEM5_HOME/util/m5
   scons build/x86/out/m5
   ```

2. Build the RNG library (existing step):

   ```
   cd $STOCHSUITE_HOME/rngs/software/
   make
   ```

3. Build workload binaries:

<<<<<<< Updated upstream
   ```
   cd $STOCHSUITE_HOME/apps/
   make gem5_fs
   ```
=======
   - **SE** (no `libm5`, no disk image):
>>>>>>> Stashed changes

     ```
     cd $STOCHSUITE_HOME/apps/
     make se
     ```

     Produces `apps/<benchmark>_se` and `apps/<benchmark>_se.stripped`
     (the stripped copy is used by `StochBranchMonitor`).

   - **FS** — the six workloads use `LFLAGS_FS` (same flags as
     `stochsuite.mk` but **without `-static`**) plus `libm5`; utilities
     such as `util/correctness` may still use `-static`. All are built
     with `-g`:

     ```
     cd $STOCHSUITE_HOME/apps/
     make
     ```

4. **(FS only)** Obtain a base disk image. The cleanest option is to use gem5's
   `obtain-resource.py` to download a fresh copy of the canned
   `x86-ubuntu-24.04-img` to a stochsuite-specific path:

   ```
   mkdir -p $STOCHSUITE_HOME/disk_images
   cd $GEM5_HOME
   build/ALL/gem5.opt util/obtain-resource.py \
       x86-ubuntu-24.04-img \
       -p $STOCHSUITE_HOME/disk_images/stochsuite-base.img
   ```

   The download is a few GiB and ends up as a raw image at
   `$STOCHSUITE_HOME/disk_images/stochsuite-base.img`.

   If you already have a base image elsewhere on disk that you do not
   mind mutating, copy it instead:

   ```
   mkdir -p $STOCHSUITE_HOME/disk_images
   cp /path/to/some-x86-ubuntu-24.04.img \
      $STOCHSUITE_HOME/disk_images/stochsuite-base.img
   ```

   Either way, **do not point the next step at a shared image** — the
   helper script writes the workload binaries into the image in place.

5. **(FS only)** Install the binaries into the disk image. The helper script
   loop-mounts the image, copies each `*.o` into
   `/home/gem5/stochsuite/<name>_gem5`, and unmounts:

   ```
   sudo $STOCHSUITE_HOME/tools/populate_disk_image.sh \
       $STOCHSUITE_HOME/disk_images/stochsuite-base.img \
       $GEM5_HOME \
       $STOCHSUITE_HOME
   ```

If you see `Makefile:1: /stochsuite.mk: No such file or directory`, it
means `$STOCHSUITE_HOME` was empty when `make` ran — re-export it in the
current shell.

Build the gem5 binary once (from `$GEM5_HOME`):

```
scons build/ALL/gem5.opt -j$(nproc)
```

## Running SE mode

From `$GEM5_HOME`, point gem5 at the SE config and a built `*_se` binary
(resolved via `$STOCHSUITE_HOME`, or a sibling `stochsuite/` tree next to
`configs/` if the variable is unset):

```
./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000

# Software PRNG in the app (no RDRAND):
./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000 --prng-select JKISS

# Hardened PRNG: app uses HWRNG (RDRAND); gem5 implements it with --prng-select:
./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000 --harden-prng --prng-select Taus88

# Classic caches + StridePrefetcher on L1D:
./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --mem-system classic --l1d-hwp-type StridePrefetcher \
    --benchmark pi --iters 1000
```

Discovery helpers (print available types and exit):

```
./build/ALL/gem5.opt configs/x86-se-pi.py --list-bp-types
./build/ALL/gem5.opt configs/x86-se-pi.py --list-hwp-types
./build/ALL/gem5.opt configs/x86-se-pi.py --list-prefetcher-types
```

When the run finishes, stats are in `m5out/stats.txt` (or the directory
passed to gem5 with `-d <dir>`).

### SE command-line options (`x86-se-pi.py`)

| Option | Default | Description |
|--------|---------|-------------|
| `--benchmark` | *(required)* | Workload: `pi`, `dop`, `dropout`, `multinomial`, `photon`, `tailwag`. Runs `apps/<name>_se`. |
| `--iters` | `100000` | Passed to the workload as `-iters`. |
| `--seed` | `12312332` | Passed to the workload as `-seed`. |
| `--prng-select` | `Taus88` | Without `--harden-prng`: passed to the app as `-rng`. With `--harden-prng`: selects the gem5 backend for RDRAND. Names match `RNGFactory.cpp` (e.g. `Taus88`, `JKISS`, `MersenneTwister`, `XorShift128`). |
| `--harden-prng` | off | App uses `HWRNG` (RDRAND); gem5 models it using `--prng-select`. |
| `--hwrng-lat` | `1` | O3 functional-unit latency for `RdRand` (only with `--harden-prng`). |
| `--rdseed-lat` | `1` | O3 functional-unit latency for `RdSeed`. |
| `--mem-system` | `ruby` | `ruby` (MESI two-level) or `classic` (private L1, shared L2). |
| `--bp-type` | `LocalBP` | Conditional branch predictor (`--list-bp-types` for choices). |
| `--prefetcher-type` | `none` | **Ruby only:** L1 data prefetcher (`none` or `RubyPrefetcher`; `--list-prefetcher-types`). |
| `--l1d-hwp-type` | *(cache default)* | **Classic only:** L1D HW prefetcher (`none` disables; `--list-hwp-types`). |
| `--l1i-hwp-type` | *(cache default)* | **Classic only:** L1I HW prefetcher. |
| `--l2-hwp-type` | *(cache default)* | **Classic only:** L2 HW prefetcher. |
| `--list-bp-types` | — | List branch predictors and exit. |
| `--list-hwp-types` | — | List classic prefetcher types and exit. |
| `--list-prefetcher-types` | — | List Ruby prefetcher types and exit. |

Prefetch flags are mutually exclusive by memory system: use
`--prefetcher-type` with `--mem-system ruby`, or `--l1d-hwp-type` /
`--l1i-hwp-type` / `--l2-hwp-type` with `classic`.

## Running FS mode

```
./build/ALL/gem5.opt \
    configs/x86-fs-stochsuite-workloads.py \
    --benchmark pi --iters 1000 \
    --disk-image $STOCHSUITE_HOME/disk_images/stochsuite-base.img

# Software PRNG in the guest (no RDRAND):
./build/ALL/gem5.opt \
    configs/x86-fs-stochsuite-workloads.py \
    --benchmark pi --iters 1000 --prng-select JKISS \
    --disk-image $STOCHSUITE_HOME/disk_images/stochsuite-base.img

# Hardened PRNG: app uses HWRNG (RDRAND); gem5 implements it with --prng-select:
./build/ALL/gem5.opt \
    configs/x86-fs-stochsuite-workloads.py \
    --benchmark pi --iters 1000 --harden-prng --prng-select Taus88 \
    --disk-image $STOCHSUITE_HOME/disk_images/stochsuite-base.img
```

Stats for the ROI are written to `m5out/stats.txt` at WORKEND.

### FS command-line options (`x86-fs-stochsuite-workloads.py`)

RNG options match SE (`--prng-select`, `--harden-prng`, `--hwrng-lat`).

| Option | Default | Description |
|--------|---------|-------------|
| `--benchmark` | *(required)* | Same six workloads as SE; runs the `*_gem5` binary on the disk image. |
| `--iters` | `100000` | Passed to the workload as `-iters`. |
| `--seed` | `12312332` | Passed to the workload as `-seed`. |
| `--prng-select` | `Taus88` | Without `--harden-prng`: passed to the app as `-rng`. With `--harden-prng`: selects the gem5 backend for RDRAND. |
| `--harden-prng` | off | App uses `HWRNG` (RDRAND); gem5 models it using `--prng-select`. |
| `--hwrng-lat` | `1` | O3 functional-unit latency for `RdRand` (only with `--harden-prng`). |
| `--rdseed-lat` | `1` | O3 latency for `RdSeed`. |
| `--disk-image` | *(required)* | Path to the populated Ubuntu image. |
| `--root-partition` | `2` | Rootfs partition (`2` for GPT `x86-ubuntu-24.04-img`; use `1` for legacy MBR). |
| `--disk-device` | `/dev/sda` | Guest disk device (kernel cmdline uses `root=<device><partition>`). |
| `--kernel` | `x86-linux-kernel-6.8.0-52-generic` | gem5 resource id or local path for the Linux kernel. |
| `--mem-system` | `ruby` | Same as SE: `ruby` or `classic`. |
| `--bp-type` | `LocalBP` | Same as SE (`--list-bp-types`). |
| `--prefetcher-type` | `none` | Same as SE (`--list-prefetcher-types`). |
| `--l1d-hwp-type` | *(cache default)* | Same as SE (`--list-hwp-types`). |
| `--l1i-hwp-type` | *(cache default)* | Same as SE. |
| `--l2-hwp-type` | *(cache default)* | Same as SE. |
| `--list-bp-types` | — | Same as SE. |
| `--list-hwp-types` | — | Same as SE. |
| `--list-prefetcher-types` | — | Same as SE. |
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
