import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("latency_stats.csv")

fig, axes = plt.subplots(1, 3, figsize=(14, 4), sharex=False)

# 1. Bar chart
colors = plt.cm.tab20.colors
axes[0].bar(df['RNG'], df['Mean'], color=colors[0])
axes[0].set_ylim(0, 200)
axes[0].set_xticklabels(df['RNG'], rotation=90)
axes[0].set_title("a) Mean Latency", fontweight='bold')
axes[0].set_ylabel("Latency per 100 RNG\nSamples (cycles)")

# Find HWRNG position
hwrng_idx = df[df['RNG'] == 'RDRAND'].index[0]
hwrng_val = df.loc[hwrng_idx, 'Mean']
axes[0].text(hwrng_idx, 185, f"{hwrng_val:.1f}", ha='center', color='red', fontweight='bold')

# 2. Box plots
bxp_stats = []
for idx, row in df.iterrows():
    bxp_stats.append({
        'label': row['RNG'],
        'med': row['Median'],
        'q1': row['FirstQuartile'],
        'q3': row['ThirdQuartile'],
        'whislo': row['Min'],
        'whishi': row['Max']
    })
axes[1].bxp(bxp_stats, showfliers=False)
axes[1].set_xticklabels(df['RNG'], rotation=90)
axes[1].set_title("b) Distribution of Latency", fontweight='bold')
# Let's use a log scale for boxplot since max values are huge (~1500-2900)
axes[1].set_yscale('log')
#axes[1].set_ylabel("Latency per RNG Sample (cycles)")

# 3. Tail Latencies
percentiles = ['P90_0', 'P95_0', 'P99_0', 'P99_9']
labels = ['P90', 'P95', 'P99', 'P99.9']

for i, pct in enumerate(percentiles):
    axes[2].plot(df['RNG'], df[pct], marker='o', label=labels[i], color=colors[i+1])
axes[2].set_xticklabels(df['RNG'], rotation=90)
axes[2].set_title("c) Tail Latencies", fontweight="bold")
#axes[2].set_ylabel("Latency per RNG Sample (cycles)")
axes[2].legend()
axes[2].set_yscale('log')

plt.tight_layout()
plt.savefig('sw_latencies.pdf', format="pdf", bbox_inches="tight")
print("Success")
