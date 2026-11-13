#!/bin/bash
#SBATCH --job-name=hw05_task3
#SBATCH --partition=research
#SBATCH --output=task3_%j.out
#SBATCH --error=task3_%j.err
#SBATCH --ntasks=1
#SBATCH --gres=gpu:rtx4000ada:1
#SBATCH --time=04:00:00

module load nvidia/cuda

cd ~/repo759/HW05

nvcc task3.cu vscale.cu -Xcompiler -O3 -Xcompiler -Wall -Xptxas -O3 -std=c++17 -o task3

echo "n,threads,time_ms,first,last" > task3_results.csv

for threads in 512 16; do
    for p in $(seq 10 29); do
        n=$((2**p))

        echo "Running n=$n threads=$threads"

        output=$(./task3 "$n" "$threads") || exit 1

        time_ms=$(echo "$output" | sed -n '1p')
        first=$(echo "$output" | sed -n '2p')
        last=$(echo "$output" | sed -n '3p')

        echo "$n,$threads,$time_ms,$first,$last" >> task3_results.csv
    done
done

echo "Task 3 sweep complete."