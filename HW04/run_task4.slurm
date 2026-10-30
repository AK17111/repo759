#!/bin/bash
#SBATCH --job-name=nbody-bench
#SBATCH --output=task4-%j.out
#SBATCH --error=task4-%j.err
#SBATCH --time=02:00:00
#SBATCH --cpus-per-task=8
#SBATCH --mem=2G

cd "$SLURM_SUBMIT_DIR"

g++ task3.cpp -Wall -O3 -std=c++17 -fopenmp -o task3 || exit 1

N=100
T=100

echo "schedule,threads,time_ms" > task4_results.csv

for schedule in static dynamic guided
do
    for threads in {1..8}
    do
        echo "Running $schedule with $threads threads"

        output=$(OMP_SCHEDULE=$schedule ./task3 "$N" "$T" "$threads")
        echo "$output"

        runtime=$(echo "$output" | awk '/^time:/ {print $2}' | sed 's/ms//')

        if [ -z "$runtime" ]; then
            echo "Could not extract runtime"
            exit 1
        fi

        echo "$schedule,$threads,$runtime" >> task4_results.csv
    done
done

echo "Benchmark results:"
cat task4_results.csv
