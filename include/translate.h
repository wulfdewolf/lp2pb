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
    void add_single(int name, int weight, bool sign);
    void add_series(int names[], int weights[], int start, int end, bool sign);
    void add_constraint_with_extra(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign);
    void add_constraint(int variables[], int weights[], int negatives, int positives, int value);

    // Streams
    stringstream constraints;
    stringstream to_sat;


    public: 

    // Executor
    Executor *executor;

    int highest = 1;
    int amount_of_constraints = 0;
    int amount_of_models;
    char* symbol_table;
    int* values;

    // Specific translation per rule type
    void translate_zero(istringstream &iss, string line);
    void translate_basic(istringstream &iss, string line);
    void translate_constraint(istringstream &iss, string line);
    void translate_choice(istringstream &iss, string line);
    void translate_weight(istringstream &iss, string line);
    void translate_min_max(istringstream &iss, string line);

    // Rule translation function container
    typedef void (Translator::*rule_translator)(istringstream &iss, string line);

    rule_translator rule_translation_functions[6] = { &Translator::translate_basic, 
                                                      &Translator::translate_constraint,
                                                      &Translator::translate_choice,
                                                      &Translator::translate_zero,
                                                      &Translator::translate_weight,
                                                      &Translator::translate_min_max };

    // Values
    void translate_values();

    // Call to lp2sat and merge
    void translate_sat();
    void merge();

};

#endif