import csv
import matplotlib.pyplot as plt

results = {
    "static": {"threads": [], "time": []},
    "dynamic": {"threads": [], "time": []},
    "guided": {"threads": [], "time": []},
}

with open("task4_results.csv", "r", newline="") as file:
    reader = csv.DictReader(file)

    for row in reader:
        policy = row["schedule"]
        results[policy]["threads"].append(int(row["threads"]))
        results[policy]["time"].append(float(row["time_ms"]))

fig, axes = plt.subplots(1, 3, figsize=(15, 5))

for ax, policy in zip(axes, ["static", "dynamic", "guided"]):
    threads = results[policy]["threads"]
    times = results[policy]["time"]

    ax.plot(threads, times, marker="o")
    ax.set_title(policy.capitalize() + " Scheduling")
    ax.set_xlabel("Number of Threads")
    ax.set_ylabel("Simulation Time (ms)")
    ax.set_xticks(range(1, 9))
    ax.grid(True)

fig.suptitle("N-body Simulation: OpenMP Scheduling Comparison")
fig.tight_layout()
fig.savefig("task4.pdf", format="pdf", bbox_inches="tight")
plt.show()