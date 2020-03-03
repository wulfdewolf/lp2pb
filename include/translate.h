#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// TRANSLATOR CLASS
class Translator {

    private:

    // Utility
    void read_literals(int array[], int amount, istringstream &iss);


    public: 

    int highest = 0;

    // Used to translate the basic rules (no aggregates) to pure SAT
    int to_sat();

    // Specific translation per rule type
    void translate_basic(istringstream &iss);
    void translate_constraint(istringstream &iss);
    void translate_choice(istringstream &iss);
    void translate_weight(istringstream &iss);
    void translate_min_max(istringstream &iss);

};

#endif