import csv
import matplotlib.pyplot as plt

data = {}

with open("scaling_results.csv", "r") as f:
    reader = csv.DictReader(f)
    for row in reader:
        rng = row["rng"]
        iters = int(row["iters"])
        avg_sec = float(row["avg_sec"])

        if rng not in data:
            data[rng] = []

        data[rng].append((iters, avg_sec))

plt.figure()

for rng in data:
    points = sorted(data[rng], key=lambda x: x[0])
    x = [p[0] for p in points]
    y = [p[1] for p in points]
    plt.plot(x, y, marker='o', label=rng)

plt.xlabel("Iterations")
plt.ylabel("Runtime (seconds)")
plt.title("Scaling Behavior of RNGs")
plt.legend()
plt.tight_layout()
plt.savefig("scaling_plot.png", dpi=300)
plt.show()