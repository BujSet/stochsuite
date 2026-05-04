import subprocess
import re
import csv
from statistics import mean

rngs = [
    "Taus88",
    "JKISS",
    "JKISS32",
    "Taus113",
    "XorShift128",
    "XorShift32",
    "XorWow",
    "CONG"
]

# 这里放你要测试的不同 iteration
iters_list = [
    10000000,
    50000000,
    100000000,
    500000000,
    1000000000
]

seed = 20020909
repeats = 3

def parse_real_time(output: str) -> float:
    """
    from:
    real    0m1.549s
    to:
    1.549
    """
    match = re.search(r"real\s+(\d+)m([\d.]+)s", output)
    if not match:
        raise ValueError(f"Cannot find real time in output:\n{output}")
    minutes = int(match.group(1))
    seconds = float(match.group(2))
    return 60 * minutes + seconds

results = []

for rng in rngs:
    print(f"\n==================== {rng} ====================")

    for iters in iters_list:
        times = []
        pis = []

        print(f"\n--- iters = {iters} ---")

        for i in range(repeats):
            cmd = f"time ./pi.o -iters {iters} -rng {rng} -seed {seed}"
            print(f"Run {i+1}: {cmd}")

            completed = subprocess.run(
                ["bash", "-lc", cmd],
                capture_output=True,
                text=True
            )

            full_output = completed.stdout + completed.stderr
            print(full_output)

            real_time = parse_real_time(full_output)
            times.append(real_time)

            pi_match = re.search(r"pi\s*=\s*([\d.]+)", full_output)
            pi_val = float(pi_match.group(1)) if pi_match else None
            pis.append(pi_val)

        avg_time = mean(times)

        row = {
            "rng": rng,
            "iters": iters,
            "run1_sec": times[0] if len(times) > 0 else None,
            "run2_sec": times[1] if len(times) > 1 else None,
            "run3_sec": times[2] if len(times) > 2 else None,
            "avg_sec": avg_time,
            "pi1": pis[0] if len(pis) > 0 else None,
            "pi2": pis[1] if len(pis) > 1 else None,
            "pi3": pis[2] if len(pis) > 2 else None,
        }

        results.append(row)
        print(f"Saved row: {row}")

print("\n===== Final Results =====")
for row in results:
    print(row)

with open("scaling_results.csv", "w", newline="") as f:
    writer = csv.DictWriter(
        f,
        fieldnames=[
            "rng", "iters",
            "run1_sec", "run2_sec", "run3_sec", "avg_sec",
            "pi1", "pi2", "pi3"
        ]
    )
    writer.writeheader()
    writer.writerows(results)

print("\nSaved to scaling_results.csv")