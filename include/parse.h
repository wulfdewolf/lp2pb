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
    void parse_rules(istream& in);
    void parse_symbol_table(istream& in);
    void parse_compute(istream& in);


    public: 

    // Translator
    Translator *translator;

    void parse(string inputfile);

};

#endif