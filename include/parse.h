#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "translate.h"
#include "util.h"

using namespace std;

// RULE CATEGORIES (smodels)
enum rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};

// PARSER CLASS
class Parser {

    private:

    // Specific utility
    void parse_max(int amount, istringstream &iss);

    // Parse hierarchy
    void parse_rules(istream& in);
    void parse_symbol_table(istream& in);
    void parse_compute(istream& in, bool sign);
    void parse_amount_of_models(istream& in);


    public: 

    // Translator
    Translator *translator;

    void parse(string inputfile);

};

#endif