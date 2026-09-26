#include "scan.h"
#include<iostream>
#include<cstdlib>
#include <vector>


std::vector<int> scan(std::vector<int>& input){
    std::vector<int> output; 
    int sum = 0; 
    for(std::size_t i = 0; i < input.size(); i++){
        sum += input[i]; 
        output[i] = sum; 
    }
    return output;
}
