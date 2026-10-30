#include <cstddef>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <random>
#include <string>
#include <omp.h>

using std::chrono::high_resolution_clock;
using std::chrono::duration;

// Constants
const double G = 1.0;
const double softening = 0.1;
const double dt = 0.01;
const double board_size = 4.0;

// Parallel acceleration calculation
void getAcc(const double pos[][3], const double mass[],
            double acc[][3], int N) {

    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < N; i++) {
        acc[i][0] = 0.0;
        acc[i][1] = 0.0;
        acc[i][2] = 0.0;

        for (int j = 0; j < N; j++) {
            if (i != j) {
                double dx = pos[j][0] - pos[i][0];
                double dy = pos[j][1] - pos[i][1];
                double dz = pos[j][2] - pos[i][2];

                double r2 = dx * dx + dy * dy + dz * dz
                            + softening * softening;

                double inv_r3 = 1.0 / (r2 * std::sqrt(r2));

                acc[i][0] += G * dx * inv_r3 * mass[j];
                acc[i][1] += G * dy * inv_r3 * mass[j];
                acc[i][2] += G * dz * inv_r3 * mass[j];
            }
        }
    }
}

// For debug: save positions to a CSV file
void savePositionsToCSV(const double pos[][3], int N, int step,
                        const std::string &filename) {
    std::ofstream file;
    file.open(filename, std::ios_base::app);

    if (file.is_open()) {
        file << step << ",[";
        for (int i = 0; i < N; i++) {
            if (i != N - 1)
                file << "[" << pos[i][0] << "," << pos[i][1]
                     << "," << pos[i][2] << "],";
            else
                file << "[" << pos[i][0] << "," << pos[i][1]
                     << "," << pos[i][2] << "]";
        }
        file << "]\n";
        file.close();
    } else {
        std::cerr << "Unable to open file!" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_sec;

    start = high_resolution_clock::now();

    // Check command-line arguments.
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <number_of_particles> <simulation_end_time>"
                  << " <num_threads>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    double tEnd = std::stod(argv[2]);
    int num_threads = std::stoi(argv[3]);

    if (N <= 0 || tEnd <= 0.0 ||
        num_threads < 1 || num_threads > 8) {
        std::cerr << "Invalid arguments. Use positive N and end time, "
                  << "and between 1 and 8 threads." << std::endl;
        return 1;
    }

    omp_set_num_threads(num_threads);

    std::string filename = "positions.csv";

    // Clear debug output file.
    std::ofstream file(filename, std::ofstream::out | std::ofstream::trunc);
    file.close();

    // Allocate dynamic arrays.
    double* mass = new double[N];
    double(*pos)[3] = new double[N][3];
    double(*vel)[3] = new double[N][3];
    double(*acc)[3] = new double[N][3];

    // Fixed seed gives reproducible initial conditions.
    std::mt19937 generator(17);
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    std::normal_distribution<double> normal_dist(0.0, 1.0);

    double t = 0.0;

    // Initial masses, positions, and velocities.
    for (int i = 0; i < N; i++) {
        mass[i] = uniform_dist(generator);

        for (int d = 0; d < 3; d++) {
            pos[i][d] = normal_dist(generator);
            vel[i][d] = normal_dist(generator);
        }
    }

    // Convert to center-of-mass frame.
    double velCM[3] = {0.0, 0.0, 0.0};
    double totalMass = 0.0;

    for (int i = 0; i < N; i++) {
        for (int d = 0; d < 3; d++) {
            velCM[d] += vel[i][d] * mass[i];
        }
        totalMass += mass[i];
    }

    for (int d = 0; d < 3; d++) {
        velCM[d] /= totalMass;
    }

    for (int i = 0; i < N; i++) {
        for (int d = 0; d < 3; d++) {
            vel[i][d] -= velCM[d];
        }
    }

    // Initial accelerations.
    getAcc(pos, mass, acc, N);

    int Nt = static_cast<int>(tEnd / dt);

    // Main simulation loop.
    for (int step = 0; step < Nt; step++) {

        // First half kick.
        #pragma omp parallel for schedule(runtime)
        for (int i = 0; i < N; i++) {
            for (int d = 0; d < 3; d++) {
                vel[i][d] += acc[i][d] * dt / 2.0;
            }
        }

        // Drift and board boundary checks.
        #pragma omp parallel for schedule(runtime)
        for (int i = 0; i < N; i++) {
            for (int d = 0; d < 3; d++) {
                pos[i][d] += vel[i][d] * dt;

                if (pos[i][d] > board_size)
                    pos[i][d] = board_size;

                if (pos[i][d] < -board_size)
                    pos[i][d] = -board_size;
            }
        }

        // Recalculate accelerations in parallel.
        getAcc(pos, mass, acc, N);

        // Second half kick.
        #pragma omp parallel for schedule(runtime)
        for (int i = 0; i < N; i++) {
            for (int d = 0; d < 3; d++) {
                vel[i][d] += acc[i][d] * dt / 2.0;
            }
        }

        t += dt;

        // Optional debug output; leave disabled for timing.
        // savePositionsToCSV(pos, N, step, filename);
    }

    delete[] mass;
    delete[] pos;
    delete[] vel;
    delete[] acc;

    end = high_resolution_clock::now();
    duration_sec =
        std::chrono::duration_cast<duration<double, std::milli>>(end - start);

    std::cout << "time: " << duration_sec.count() << "ms\n";

    return 0;
}
