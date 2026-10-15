
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

thresholds = []
times = []

with open("task3_ts_results.txt") as f:
    for line in f:
        ts, ms = line.split()
        thresholds.append(int(ts))
        times.append(float(ms))

if len(thresholds) != 10:
    raise ValueError(f"Expected 10 measurements, found {len(thresholds)}")

plt.figure()
plt.plot(thresholds, times, marker="o")
plt.xlabel("Threshold (ts)")
plt.ylabel("Execution Time (ms)")
plt.title("Parallel Merge Sort: Runtime vs. Threshold")
plt.xscale("linear")
plt.yscale("log")
plt.xticks(thresholds, [str(x) for x in thresholds])
plt.grid(True, which="both")
plt.tight_layout()
plt.savefig("task3_ts.pdf")
