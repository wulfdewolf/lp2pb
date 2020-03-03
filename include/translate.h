#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
 #include <vector>

using namespace std;

// TRANSLATOR CLASS
class Translator {

    private:

    // Utility
    void read_literals(int array[], int amount, istringstream &iss);

    string exec(string cmd);

    stringstream constraints;
    stringstream to_sat;

    public: 

    int highest = 0;
    int amount_of_constraints = 0;
    int amount_of_models;
    char* symbol_table;
    int* values;

    // Specific translation per rule type
    void translate_basic(istringstream &iss, string line);
    void translate_constraint(istringstream &iss);
    void translate_choice(istringstream &iss);
    void translate_weight(istringstream &iss);
    void translate_min_max(istringstream &iss);

    // Values
    void translate_values();

    // Call to lp2sat and merge
    void translate_sat();

};

#endif