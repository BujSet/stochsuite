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

# gem5 Full-System Simulation

Stochsuite workloads can be studied in gem5 two ways:

| Mode | Binary | CPU | OS | Config |
|---|---|---|---|---|
| **SE** (syscall emulation) | `apps/<bench>_se` | O3 only | None | `configs/x86-se-pi.py` |
| **FS** (full system) | `*_gem5` on disk image | KVM + O3 (switchable) | Ubuntu 24.04 | `configs/x86-fs-stochsuite-workloads.py` |

**Monitors**

- `StochBranchMonitor` — tracks `__stoch_br_*` sites (pi, dop, dropout, etc.)
  on the O3 branch predictor.
- `StochPrefetchMonitor` — tracks `__stoch_mem_*` sites (SGD) on the classic
  L1D cache. Requires `--mem-system classic`.

**Prefetchers**

- **Classic** (`--mem-system classic`, default in FS): gem5 hardware prefetchers
  (`StridePrefetcher`, `BOPPrefetcher`, etc.) via `--l1d-hwp-type`,
  `--l1i-hwp-type`, `--l2-hwp-type`.

### Prerequisites

```
export STOCHSUITE_HOME=/absolute/path/to/stochsuite
export GEM5_HOME=/absolute/path/to/gem5
```

For this checkout:

```
export GEM5_HOME=$HOME/work/stochsuite_gem5/gem5
export STOCHSUITE_HOME=$HOME/work/stochsuite_gem5/stochsuite
```

1. Build `libm5` (required for FS):

   ```
   cd $GEM5_HOME/util/m5
   scons build/x86/out/m5
   ```

2. Build the RNG library (see above).

    cd $STOCHSUITE_HOME/rngs/software/
   make

3. Build gem5:

   ```
   cd $GEM5_HOME
   scons build/ALL/gem5.opt -j$(nproc)
   ```

4. Build workload binaries:

   - **SE** — no `libm5`, no disk image:

     ```
     cd $STOCHSUITE_HOME/apps/
     make se
     ```

     Produces `apps/<benchmark>_se` and `apps/<benchmark>_se.stripped`.

   - **FS** — links `libm5`, enables ROI hypercalls via `m5_roi.h`:

     ```
     cd $STOCHSUITE_HOME/apps/
     make gem5_fs
     ```

     Produces `apps/<benchmark>.o` / `.stripped` and `apps/sgd/sgd.o` /
     `sgd.stripped`.

If you see `Makefile:1: /stochsuite.mk: No such file or directory`, re-export
`STOCHSUITE_HOME` in the current shell.

### FS disk image setup

1. Obtain a base Ubuntu 24.04 image:

   ```
   mkdir -p $STOCHSUITE_HOME/disk_images
   cd $GEM5_HOME
   build/ALL/gem5.opt util/obtain-resource.py \
       x86-ubuntu-24.04-img \
       -p $STOCHSUITE_HOME/disk_images/stochsuite-base.img
   ```

   Or copy an existing image:

   ```
   mkdir -p $STOCHSUITE_HOME/disk_images
   cp /path/to/x86-ubuntu-24.04.img \
      $STOCHSUITE_HOME/disk_images/stochsuite-base.img
   ```

   Do not point the populate step at a shared image you cannot overwrite.

2. Install workload binaries (six main workloads plus SGD and its data files):

   ```
   sudo $STOCHSUITE_HOME/tools/populate_disk_image.sh \
       $STOCHSUITE_HOME/disk_images/stochsuite-base.img \
       $GEM5_HOME \
       $STOCHSUITE_HOME
   ```

   Requires `make gem5_fs` in `apps/` first. SGD data files (`apps/sgd/*.txt`)
   are copied to `/home/gem5/stochsuite/sgd/` on the image.

### FS execution model

FS workloads use [apps/m5_roi.h](apps/m5_roi.h):

- **HC10** (before ROI): schedule KVM→O3 switch, NOP spin, `m5_work_begin`
- **HC11** (after ROI): `m5_work_end`, schedule O3→KVM switch, NOP spin

Boot and setup run on **KVM** (fast). The ROI runs on **O3** (detailed).
Post-ROI code switches back to **KVM** when `--mem-system classic` (default).

With `--mem-system ruby`, HC11 stays on O3 to avoid a known MESI Two Level
`memWriteback` crash on O3→KVM switches. Use classic for prefetch studies and
multi-iteration workloads like SGD.

### Running SE mode

From `$GEM5_HOME`:

```
./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000

./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000 --prng-select JKISS

./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --benchmark pi --iters 1000 --harden-prng --prng-select Taus88

./build/ALL/gem5.opt \
    configs/x86-se-pi.py \
    --mem-system classic --l1d-hwp-type StridePrefetcher \
    --benchmark pi --iters 1000
```

Discovery helpers:

```
./build/ALL/gem5.opt configs/x86-se-pi.py --list-bp-types
./build/ALL/gem5.opt configs/x86-se-pi.py --list-hwp-types
./build/ALL/gem5.opt configs/x86-se-pi.py --list-prefetcher-types
```

Stats are written to `m5out/stats.txt` (or the directory passed with `-d`).

#### SE options (`x86-se-pi.py`)

| Option | Default | Description |
|--------|---------|-------------|
| `--benchmark` | *(required)* | `pi`, `dop`, `dropout`, `multinomial`, `photon`, `tailwag` |
| `--iters` | `100000` | Passed to the workload as `-iters` |
| `--seed` | `12312332` | Passed to the workload as `-seed` |
| `--prng-select` | `Taus88` | App RNG, or gem5 RDRAND backend with `--harden-prng` |
| `--harden-prng` | off | App uses HWRNG (RDRAND) |
| `--hwrng-lat` | `1` | O3 latency for `RdRand` |
| `--rdseed-lat` | `1` | O3 latency for `RdSeed` |
| `--mem-system` | `ruby` | `ruby` or `classic` |
| `--bp-type` | `LocalBP` | Branch predictor (`--list-bp-types`) |
| `--prefetcher-type` | `none` | Ruby L1 prefetcher (`--list-prefetcher-types`) |
| `--l1d-hwp-type` | cache default | Classic L1D prefetcher (`--list-hwp-types`) |
| `--l1i-hwp-type` | cache default | Classic L1I prefetcher |
| `--l2-hwp-type` | cache default | Classic L2 prefetcher |

Use `--prefetcher-type` with `ruby`, or `--l1d-hwp-type` / `--l1i-hwp-type` /
`--l2-hwp-type` with `classic` — not both.

### Running FS mode

From `$GEM5_HOME`:

```
./build/ALL/gem5.opt \
    configs/x86-fs-stochsuite-workloads.py \
    --benchmark pi --iters 1000 \
    --disk-image $STOCHSUITE_HOME/disk_images/stochsuite-base.img

./build/ALL/gem5.opt \
    configs/x86-fs-stochsuite-workloads.py \
    --benchmark sgd --iters 1 \
    --l1d-hwp-type StridePrefetcher \
    --disk-image $STOCHSUITE_HOME/disk_images/stochsuite-base.img
```

Classic is the default memory system; `--mem-system classic` is optional.
`--l1d-hwp-type StridePrefetcher` is recommended for SGD prefetch studies.

Expected terminal output per ROI iteration:

```
HC10: switching KVM -> O3 for ROI
WorkBegin: ROI marker
WorkEnd: ROI marker
HC11: switching O3 -> KVM after ROI
```

ROI stats are dumped at HC11. gem5 may write a second stats block at
simulation exit — use the HC11 block for ROI metrics.

#### FS options (`x86-fs-stochsuite-workloads.py`)

RNG options match SE (`--prng-select`, `--harden-prng`, `--hwrng-lat`).

| Option | Default | Description |
|--------|---------|-------------|
| `--benchmark` | *(required)* | `pi`, `dop`, `dropout`, `multinomial`, `photon`, `tailwag`, `sgd` |
| `--iters` | `100000` | Passed to the workload as `-iters` |
| `--seed` | `12312332` | Passed to the workload as `-seed` |
| `--prng-select` | `Taus88` | Same as SE |
| `--harden-prng` | off | Same as SE |
| `--hwrng-lat` | `1` | Same as SE |
| `--rdseed-lat` | `1` | Same as SE |
| `--disk-image` | *(required)* | Path to the populated Ubuntu image |
| `--root-partition` | `2` | Rootfs partition (`2` for GPT; `1` for legacy MBR) |
| `--disk-device` | `/dev/sda` | Guest disk device |
| `--kernel` | `x86-linux-kernel-6.8.0-52-generic` | gem5 resource id or local kernel path |
| `--mem-system` | `classic` | `classic` or `ruby` |
| `--bp-type` | `LocalBP` | Branch predictor (`--list-bp-types`) |
| `--prefetcher-type` | `none` | Ruby L1 prefetcher (`--list-prefetcher-types`) |
| `--l1d-hwp-type` | cache default | Classic L1D prefetcher (`--list-hwp-types`) |
| `--l1i-hwp-type` | cache default | Classic L1I prefetcher |
| `--l2-hwp-type` | cache default | Classic L2 prefetcher |

---

## TODO

### Random number generators

- Yarrow, Fortuna, PCGBasic (broken), Random123, middle-squared method
- HWRNG: graceful error when DRNG is unavailable on the host

### Applications

- Additional Monte Carlo workloads (e.g. color chart integrator)
