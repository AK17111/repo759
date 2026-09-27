#!/bin/bash

#SBATCH --cpus-per-task=2
#SBATCH --job-name=Task1
#SBATCH --output=Task1.out
#SBATCH --error=Task1.err
#SBATCH --partition=instruction
#SBATCH --mem=12G

./task1 1024
./task1 2048
./task1 4096
./task1 8192
./task1 16384
./task1 32768
./task1 65536
./task1 131072
./task1 262144
./task1 524288
./task1 1048576
./task1 2097152
./task1 4194304
./task1 8388608
./task1 16777216
./task1 33554432
./task1 67108864
./task1 134217728
./task1 268435456
./task1 536870912
./task1 1073741824