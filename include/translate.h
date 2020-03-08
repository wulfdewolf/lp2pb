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

    // Streams
    stringstream constraints;
    stringstream to_sat;


    public: 

    // Executor
    Executor *executor;

    int highest = 0;
    int amount_of_constraints = 0;
    int amount_of_models;
    char* symbol_table;
    int* values;

    // Specific translation per rule type
    void translate_basic(istringstream &iss, string line);
    void translate_constraint(istringstream &iss, string line);
    void translate_choice(istringstream &iss, string line);
    void translate_weight(istringstream &iss, string line);
    void translate_min_max(istringstream &iss, string line);

    // Rule translation function container
    typedef void (Translator::*rule_translator)(istringstream &iss, string line);
    rule_translator rule_translation_functions[5] = { &Translator::translate_basic, 
                                                      &Translator::translate_constraint,
                                                      &Translator::translate_choice,
                                                      &Translator::translate_weight,
                                                      &Translator::translate_min_max };

    // Values
    void translate_values();

    // Call to lp2sat and merge
    void translate_sat();
    void merge();

};

#endif