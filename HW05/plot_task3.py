
import pandas as pd
import matplotlib.pyplot as plt

# Load Task 3 benchmark results
df = pd.read_csv("task3_results.csv")

# Convert columns to numeric
df["n"] = pd.to_numeric(df["n"])
df["threads"] = pd.to_numeric(df["threads"])
df["time_ms"] = pd.to_numeric(df["time_ms"])

# Plot both thread configurations
plt.figure(figsize=(9, 6))

for threads in [512, 16]:
    data = df[df["threads"] == threads].sort_values("n")

    plt.plot(
        data["n"],
        data["time_ms"],
        marker="o",
        label=f"{threads} threads per block"
    )

plt.xscale("log", base=2)
plt.xlabel("Array size (n)")
plt.ylabel("Execution time (ms)")
plt.title("CUDA Vector Scaling Performance")
plt.legend()
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()

# Save the required PDF
plt.savefig("task3.pdf", format="pdf")

print("Created task3.pdf")
