#!/bin/bash
#SBATCH --job-name=task3_t
#SBATCH --output=task3_t_%j.out
#SBATCH --error=task3_t_%j.err
#SBATCH --cpus-per-task=20
#SBATCH --time=00:15:00
#SBATCH --partition=instruction

set -euo pipefail

g++ task3.cpp msort.cpp -Wall -O3 -std=c++17 -o task3 -fopenmp

#best measured from previous part
BEST_TS=256

: > task3_t_results.txt

for t in {1..20}; do
    echo "Running Task 3: n=1000000, t=$t, ts=$BEST_TS"

    output=$(./task3 1000000 "$t" "$BEST_TS")
    printf '%s\n' "$output"

    time_ms=$(printf '%s\n' "$output" | sed -n '3p')

    if ! [[ "$time_ms" =~ ^[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$ ]]; then
        echo "ERROR: Invalid runtime for t=$t: $time_ms" >&2
        exit 1
    fi

    printf '%s %s\n' "$t" "$time_ms" >> task3_t_results.txt
done

echo "Finished. Results saved in task3_t_results.txt"
