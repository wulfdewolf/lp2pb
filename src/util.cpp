/*
*
*   UTILITY
*   
*/

#include "../include/util.h"

void skip(int amount, istringstream &iss) {
    int curr;
    for(int i = 0; i < amount; i++) {
        iss>>curr;
    }
    return;
}

// Command line argument parsing 
char* getCmdOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}