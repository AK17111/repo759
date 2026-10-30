#!/bin/bash
#SBATCH --job-name=nbody-test
#SBATCH --partition=research
#SBATCH --output=nbody-test-%j.out
#SBATCH --error=nbody-test-%j.err
#SBATCH --time=01:00:00
#SBATCH --cpus-per-task=8
#SBATCH --mem=2G

cd "$SLURM_SUBMIT_DIR"

echo "Compile serial task2"
g++ task2.cpp -Wall -O3 -std=c++17 -o task2 || exit 1

echo "Compile parallel task3"
g++ task3.cpp -Wall -O3 -std=c++17 -fopenmp -o task3 || exit 1

echo "===== Serial: task2 ====="
./task2 100 100

echo "===== Parallel: task3 with 8 threads ====="
OMP_SCHEDULE=static ./task3 100 100 8
