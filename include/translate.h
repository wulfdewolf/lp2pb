#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "execute.h"
#include "util.h"

using namespace std;

// EXCEPTIONS
struct executable_exception : public exception {
    const char* what() const throw() {
        return "Executable error!";
    }
};

struct invalid_outputfile_exception : public exception {
    const char* what() const throw() {
        return "Outputfile invalid.";
    }
};

// TRANSLATOR CLASS
class Translator {

    private:

    // Utility
    void read_literals(int array[], int amount, istringstream &iss, bool count = true);
    void read_weights(int array[], int amount, istringstream &iss) {
        read_literals(array, amount, iss, false);
    };
    void add_sat(int rule[], int amount);
    void add_single(int name, int weight, bool sign, stringstream &iss);

    // Allow passing the stream
    void add_series(int names[], int weights[], int start, int end, bool sign) {
        add_series(names, weights, start, end, sign, this->constraints);
    }
    void add_series(int names[], int weights[], int start, int end, bool sign, stringstream &iss);
    void add_constraint(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign);

    bool file_exists(const char *fileName);

    // Mininimise statement
    stringstream minimize;
    stringstream minimize_weights;


    public: 

    // Streams
    stringstream constraints;
    stringstream to_lp2sat;

    // Symbol table
    map<int, string> symbol_table;
    map<int, string> agg_variables_symbol_table;

    // Executor
    Executor *executor;

    // Output
    string outputfile = "pipe";

    int highest = 0;
    int amount_of_constraints = 0;
    int amount_of_models;

    // Specific translation per rule type
    void translate_sat(string line);
    void translate_constraint(istringstream &iss);
    void translate_weight(istringstream &iss);
    void translate_minimize(istringstream &iss);

    // Values
    void translate_value(int index, int sign);

    // Symbol tables
    void translate_symbol_table();

    // Aggregate variables
    int amount_of_aggregate_variables = 0;
    void add_aggregate_variables_rule();
    
    // Call to lp2sat and merge
    void merge();
};

#endif