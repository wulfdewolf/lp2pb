#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

// Some utility functions

void skip(int amount, istringstream &iss);

char* getCmdOption(char ** begin, char ** end, const std::string & option);

bool cmdOptionExists(char** begin, char** end, const std::string& option);


#endif