import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# 1. Read the data from the CSV file
df = pd.read_csv("latency_stats.csv")
 
# Extract unique and sorted categories
machines = sorted(df["Machine"].unique())
all_rngs = sorted(df["RNG"].unique())

# Separate RDRAND from the other 15 PRNG methods
non_rdrand_rngs = [r for r in all_rngs if r != "RDRAND"]

# Divide the 15 PRNGs into 3 groups of 5 for ax3, ax4, and ax5
groups = [
    non_rdrand_rngs[0:5],   # First 5 PRNGs for ax3
    non_rdrand_rngs[5:10],  # Next 5 PRNGs for ax4
    non_rdrand_rngs[10:15], # Last 5 non-RDRAND PRNGs for ax5
    ["RDRAND"]              # RDRAND on its own for ax6
]

# 2. Extract unique and sorted categories to maintain a clean order
rng_methods = sorted(df["RNG"].unique())
# Explicitly move GLIBC_CRAND to the front (index 0) so it displays on the far left
if "GLIBC_CRAND" in rng_methods:
    rng_methods.remove("GLIBC_CRAND")
    rng_methods.insert(0, "GLIBC_CRAND")
machines = sorted(df["Machine"].unique())

# 3. Create the 6 subplots across 3 rows using a 3x4 grid system
ax1 = plt.subplot2grid((3, 4), (0, 0), colspan=4)  # Row 1, spans full width
ax2 = plt.subplot2grid((3, 4), (1, 0), colspan=4)  # Row 2, spans full width
ax3 = plt.subplot2grid((3, 4), (2, 0))              # Row 3, col 1
ax4 = plt.subplot2grid((3, 4), (2, 1))              # Row 3, col 2
ax5 = plt.subplot2grid((3, 4), (2, 2))              # Row 3, col 3
ax6 = plt.subplot2grid((3, 4), (2, 3))              # Row 3, col 4

# 4. Adjust the overall figure size to ensure everything fits perfectly
fig = plt.gcf()
fig.set_size_inches(18, 10)

# 5. Define parameters for the grouped bar chart on the first subplot
x = np.arange(len(rng_methods))
width = 0.15  # Width of an individual bar (5 bars per group total 0.75 span)

# Colors and distinct hatch patterns for the 5 machines
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]
hatches = ["//", "\\\\", "||", "--", "++"]

ax1.set_ylim(0, 175)

# 6. Plot the vertical bars for each machine
for i, machine in enumerate(machines):
    # Filter data for the specific machine and align it to the sorted RNG index
    machine_df = df[df["Machine"] == machine].set_index("RNG")
    means = [machine_df.loc[rng, "Mean"] for rng in rng_methods]
    
    # Center the 5 bars perfectly around each tick position
    pos = x + (i * width) - (2 * width)
    
    ax1.bar(
        pos, 
        means, 
        width, 
        label=machine, 
        color=colors[i % len(colors)], 
        hatch=hatches[i % len(hatches)],
        edgecolor="black",
        linewidth=0.5
    )
    for r_idx, rng in enumerate(rng_methods):
        if rng == "RDRAND" and rng in machine_df.index:
            val = machine_df.loc[rng, "Mean"]
            ax1.text(
                pos[r_idx], 150, f"{int(val)}",
                color=colors[i % len(colors)],
                ha="center", va="center", rotation=90, fontweight="bold",
                bbox=dict(facecolor="white", edgecolor="none", alpha=0.9, pad=1)
            )

# 7. Format the first subplot labels and titles
ax1.set_xticks(x)
ax1.set_xticklabels(rng_methods, rotation=45, ha="right")
ax1.set_ylabel("Latency per RNG\nSample (Cycles)")
ax1.set_title("a) Mean Latency", fontweight="bold")
ax1.legend(loc='upper left')

# --- Code for the second plot on ax2 ---

# 1. Define parameters for positioning the 16 groups of 5 boxes
x = np.arange(len(rng_methods))
width = 0.15  # Width of each box plot

# Use the same color palette as the first plot for consistency
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd"]

ax2.set_ylim(0, 450)
# 2. Iterate through each machine to plot its boxes across all 16 RNG methods
for i, machine in enumerate(machines):
    stats_list = []
    positions = []

    # Filter dataframe for the current machine
    machine_df = df[df["Machine"] == machine].set_index("RNG")

    for j, rng in enumerate(rng_methods):
        if rng in machine_df.index:
            row = machine_df.loc[rng]

            # Map the CSV columns to the format required by matplotlib's bxp
            stats_list.append({
                "med": row["Median"],
                "q1": row["FirstQuartile"],
                "q3": row["ThirdQuartile"],
                "whislo": row["Min"],
                "whishi": row["Max"],
                "fliers": []  # Empty list since outliers are already bound by Min/Max
            })
#            # Calculate the horizontal shift for grouping
#positions.append(j + (i * width) - (2 * width))
            # Calculate exact X position for this specific machine's box
            box_pos = j + (i * width) - (2 * width)
            positions.append(box_pos)
            max_val = row["Max"]
            if max_val > 450 and rng != "RDRAND":
                ax2.text(
                    box_pos, 
                    370,  # Positioned internally near the top edge
                    f"Max:{int(max_val)}", 
                    color=colors[i % len(colors)], 
                    ha="center", va="center", rotation=90, fontweight="bold", fontsize=8,
                    bbox=dict(facecolor="white", edgecolor="none", alpha=0.8, pad=0.5)
                )

            # If it's RDRAND, print its median value near the top of the plot area
            if rng == "RDRAND":
                median_val = row["Median"]
                ax2.text(
                    box_pos,
                    390,
                    f"{int(median_val)}",
                    color=colors[i % len(colors)],
                    ha="center",
                    va="center",
                    rotation=90,
                    fontweight="bold",
                    bbox=dict(facecolor="white", edgecolor="none", alpha=0.8, pad=1)
                )

    # 3. Draw the box plots for the current machine
    ax2.bxp(
        stats_list,
        positions=positions,
        widths=width * 0.8,
        medianprops=dict(color=colors[i % len(colors)], linewidth=2),  # Color-coded medians
        boxprops=dict(color="black"),
        whiskerprops=dict(color="black"),
        capprops=dict(color="black")
    )

    # Create a proxy artist for the legend to associate the line color with the machine
    ax2.plot([], [], color=colors[i % len(colors)], label=machine, linewidth=2)

# 4. Format the second subplot (labels, ticks, and titles)
ax2.set_xticks(x)
ax2.set_xticklabels(rng_methods, rotation=45, ha="right")
ax2.set_ylabel("Latency per RNG\nSample (Cycles)")
ax2.set_title("b) Latency Distribution", fontweight="bold")
ax2.legend(loc='upper left')



# =====================================================================
# SUBPLOTS 3-6: Normalized Tail Latency Line Plots
# =====================================================================
axes = [ax3, ax4, ax5, ax6]
titles = [
    "c)  Normalized Tail Latency (Group 1)", 
    "d)  Normalized Tail Latency (Group 2)", 
    "e)  Normalized Tail Latency (Group 3)", 
    "f)  Normalized Tail Latency (RDRAND)"
]

# Defining percentiles for the horizontal axis scale
percentile_labels = ["$90.0\%$", "$95.0\%$", "$99.0\%$", "$99.9\%$"]
percentile_cols = ["P90_0", "P95_0", "P99_0", "P99_9"]
markers = ["o", "s", "^", "D", "v"]

for g_idx, (group, ax, title) in enumerate(zip(groups, axes, titles)):
    ax.set_title(title, fontweight="bold")
    ax.set_xlabel("Tail Latency Percentile")
    
    # Updated y-axis label to reflect the mathematical normalization
    if g_idx == 0:
        ax.set_ylabel("Normalized Tail Latency")
        
    for m_idx, machine in enumerate(machines):
        machine_df = df[df["Machine"] == machine].set_index("RNG")
        
        for r_idx, rng in enumerate(group):
            if rng in machine_df.index:
                row = machine_df.loc[rng]
                median_val = row["Median"]
                
                # Fetch absolute values
                raw_y_vals = [row[col] for col in percentile_cols]
                
                # Normalize values by dividing by the median (handling potential zero boundaries safely)
                if median_val > 0:
                    y_vals = [y / median_val for y in raw_y_vals]
                else:
                    y_vals = [0 for _ in raw_y_vals]
                
                # Plot line using machine color and a unique marker style for the PRNG
                ax.plot(
                    percentile_labels, 
                    y_vals, 
                    color=colors[m_idx % len(colors)], 
                    marker=markers[r_idx % len(markers)], 
                    linestyle="-", 
                    alpha=0.75,
                    linewidth=1.5
                )
                
    # Generate clean, non-duplicated legends for the subplots
    if group != ["RDRAND"]:
        # Subplots 3, 4, 5 display a legend mapping the marker types to the 5 PRNGs
        for r_idx, rng in enumerate(group):
            ax.plot([], [], color="black", marker=markers[r_idx % len(markers)], linestyle="-", label=rng)
        ax.legend(fontsize="small", loc="upper left", title=f"Group {g_idx+1}'s PRNGs")
    else:
        # Subplot 6 contains only RDRAND, so its legend clarifies the 5 Machine colors instead
        for m_idx, machine in enumerate(machines):
            ax.plot([], [], color=colors[m_idx % len(colors)], marker="o", linestyle="-", label=machine)
        ax.legend(fontsize="small", loc="upper left", title="Machine")

# 8. Optimize the layout to prevent truncated text and save the figure
plt.tight_layout()
plt.savefig("sw_latencies.pdf", format="pdf")
