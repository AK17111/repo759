
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

threads = []
times = []

with open("task3_t_results.txt") as f:
    for line in f:
        t, ms = line.split()
        threads.append(int(t))
        times.append(float(ms))

if len(threads) != 20:
    raise ValueError(f"Expected 20 measurements, found {len(threads)}")

plt.figure()
plt.plot(threads, times, marker="o")
plt.xlabel("Number of Threads (t)")
plt.ylabel("Execution Time (ms)")
plt.title("Parallel Merge Sort: Runtime vs. Threads")
plt.xticks(range(1, 21))
plt.grid(True)
plt.tight_layout()
plt.savefig("task3_t.pdf")
