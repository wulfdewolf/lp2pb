#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "translate.h"

using namespace std;

// RULE CATEGORIES (smodels)
enum rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};

// PARSER CLASS
class Parser {

    private:

    // Utility
    void skip(int amount, istringstream &iss);
    void parse_max(int amount, istringstream &iss);

    // Parse hierarchy
    void parse_file(char* file);
    void parse_rules(ifstream &infile);
    void parse_symbol_table(ifstream &infile);
    void parse_compute(ifstream &infile);


    public: 

    // Translator
    Translator *translator;

    void parse(char* files[], int nfiles);

};

#endif