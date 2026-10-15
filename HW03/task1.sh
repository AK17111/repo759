#!/bin/bash
#SBATCH --job-name=task1
#SBATCH --output=task1_%j.out
#SBATCH --error=task1_%j.err
#SBATCH --cpus-per-task=20
#SBATCH --time=00:15:00
#SBATCH --partition=instruction

set -euo pipefail

g++ task1.cpp matmul.cpp -Wall -O3 -std=c++17 -o task1 -fopenmp

: > task1_results.txt

for t in {1..20}; do
    echo "Running Task 1: n=1024, t=$t"

    output=$(./task1 1024 "$t")
    printf '%s\n' "$output"

    time_ms=$(printf '%s\n' "$output" | sed -n '3p')

    if ! [[ "$time_ms" =~ ^[0-9]+([.][0-9]+)?([eE][-+]?[0-9]+)?$ ]]; then
        echo "ERROR: Invalid runtime for t=$t: $time_ms" >&2
        exit 1
    fi

    printf '%s %s\n' "$t" "$time_ms" >> task1_results.txt
done

echo "Finished. Results saved in task1_results.txt"
