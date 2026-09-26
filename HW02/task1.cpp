#include<iostream>
#include<cstdlib>
// std::ratio provides easy conversions between metric units
#include <ratio>
#include "scan.h"


// Provide some namespace shortcuts
using std::cout;
using std::chrono::high_resolution_clock;
using std::chrono::duration;


int main(int argc, char *argv[]) {
    //random array part///////////////////////////////////////////////// 
    int n = std::stoi(argv[1]);
    float* arr = new float[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = -1.0f + 2.0f * rand() / RAND_MAX;
    }

    //timer part///////////////////////////////////////////////////////
    float* output = new float[n]; 
    high_resolution_clock::time_point start;
    high_resolution_clock::time_point end;
    duration<double, std::milli> duration_sec;

    // Get the starting timestamp
    start = high_resolution_clock::now();
    output = scan(arr, n);
   // Get the ending timestamp
    end = high_resolution_clock::now();

    // Convert the calculated duration to a double using the standard library
    duration_sec = std::chrono::duration_cast<duration<double, std::milli>>(end - start);
    // Durations are converted to milliseconds already thanks to std::chrono::duration_cast
    cout << duration_sec.count() << "\n";
    cout << output[0] << "\n";
    cout << output[n-1] << "\n";

    delete[] output;
    delete[] arr;

    return 0;
    
}