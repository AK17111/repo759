
#!/bin/bash
#SBATCH --job-name=task3_ts
#SBATCH --output=task3_ts_%j.out
#SBATCH --error=task3_ts_%j.err
#SBATCH --cpus-per-task=8
#SBATCH --time=00:15:00
#SBATCH --partition=instruction

set -euo pipefail

g++ task3.cpp msort.cpp -Wall -O3 -std=c++17 -o task3 -fopenmp

: > task3_ts_results.txt

for ts in 2 4 8 16 32 64 128 256 512 1024; do
    echo "Running Task 3: n=1000000, t=8, ts=$ts"

    output=$(./task3 1000000 8 "$ts")
    printf '%s\n' "$output"

    time_ms=$(printf '%s\n' "$output" | sed -n '3p')

    if ! [[ "$time_ms" =~ ^[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$ ]]; then
        echo "ERROR: Invalid runtime for ts=$ts: $time_ms" >&2
        exit 1
    fi

    printf '%s %s\n' "$ts" "$time_ms" >> task3_ts_results.txt
done

echo "Finished. Results saved in task3_ts_results.txt"
