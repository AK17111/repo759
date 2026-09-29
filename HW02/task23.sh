#!/bin/bash

#SBATCH --cpus-per-task=2
#SBATCH --job-name=Task23
#SBATCH --output=Task23.out
#SBATCH --error=Task23.err
#SBATCH --partition=instruction
#SBATCH --mem=4G

g++ convolution.cpp task2.cpp -Wall -O3 -std=c++17 -o task2
g++ task3.cpp matmul.cpp -Wall -O3 -std=c++17 -o task3

echo "task2 (n=1024, m=3):"
./task2 1024 3
echo "task3:"
./task3
