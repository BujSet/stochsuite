#!/usr/bin/env python3
"""
One-time conversion of SGD text data files to raw binary (little-endian float64).

Usage:
    python3 convert_to_binary.py [data_dir]

    data_dir: path containing the sgd/ subdirectory with the .txt files.
              Defaults to the current working directory (i.e. the directory
              from which the SGD binary is launched, so that relative paths
              like "sgd/y_train.txt" resolve correctly).

Output files are placed alongside the input files:
    sgd/y_train.txt  →  sgd/y_train.bin  (3,999,805 float64 values, ~32 MB)
    sgd/y_val.txt    →  sgd/y_val.bin    (1,714,203 float64 values, ~14 MB)
"""

import os
import struct
import sys


FILES = [
    ("sgd/y_train.txt", "sgd/y_train.bin"),
    ("sgd/y_val.txt",   "sgd/y_val.bin"),
]


def convert(txt_path: str, bin_path: str) -> None:
    print(f"Converting {txt_path} -> {bin_path} ...", flush=True)
    values = []
    with open(txt_path, "r") as fin:
        for line in fin:
            line = line.strip()
            if line:
                values.append(float(line))

    # Write as raw little-endian double array (same layout as C double[])
    with open(bin_path, "wb") as fout:
        fout.write(struct.pack(f"<{len(values)}d", *values))

    print(f"  wrote {len(values)} doubles ({os.path.getsize(bin_path) / 1e6:.1f} MB)", flush=True)


def main() -> None:
    data_dir = sys.argv[1] if len(sys.argv) > 1 else "."
    for txt_rel, bin_rel in FILES:
        txt_path = os.path.join(data_dir, txt_rel)
        bin_path = os.path.join(data_dir, bin_rel)
        if not os.path.exists(txt_path):
            print(f"WARNING: source file not found, skipping: {txt_path}")
            continue
        convert(txt_path, bin_path)
    print("Done.")


if __name__ == "__main__":
    main()
