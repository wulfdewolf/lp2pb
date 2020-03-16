#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "execute.h"

using namespace std;

// TRANSLATOR CLASS
class Translator {

    private:

    // Utility
    void read_literals(int array[], int amount, istringstream &iss);
    void add_sat(int rule[], int amount);
    void add_single(int name, int weight, bool sign, stringstream &iss);

    // Allow passing the stream
    void add_series(int names[], int weights[], int start, int end, bool sign) {
        add_series(names, weights, start, end, sign, this->constraints);
    }
    void add_series(int names[], int weights[], int start, int end, bool sign, stringstream &iss);

    void add_constraint_with_extra(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign);
    void add_constraint(int variables[], int weights[], int negatives, int positives, int value);

    // Mininimise statement
    stringstream minimize;


    public: 

    // Streams
    stringstream constraints;
    stringstream to_sat;

    // Executor
    Executor *executor;

    // Output
    string outputfile = "pipe";

    int highest = 1;
    int amount_of_constraints = 0;
    int amount_of_models;
    char* symbol_table;

    // Specific translation per rule type
    void translate_sat(string line);
    void translate_constraint(istringstream &iss);
    void translate_weight(istringstream &iss);
    void translate_minimize(istringstream &iss);

    // Values
    void translate_value(int index, int sign);

    // Call to lp2sat and merge
    void merge();

};

#endif