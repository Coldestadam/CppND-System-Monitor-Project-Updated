#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string HH, MM, SS;
    string output;

    HH = std::to_string(seconds / 3600);
    seconds -= std::stoi(HH) * 3600;

    MM = std::to_string(seconds / 60);
    seconds -= std::stoi(MM) * 60;

    SS = std::to_string(seconds);
    SS.insert(0, 2 - SS.length(), '0');


    output = HH + ':' + MM + ':' + SS; 
    return output;
}