#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// RULE CATEGORIES (smodels)
enum class rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};

// PARSING STATUS
enum class parse_status{ RULES_DONE};

// PARSER CLASS
class Parser {

    private:

    int amount_of_variables = 0;
    int amount_of_contraints = 0;

    int parse_file(char* file);

    int read_symbol_table();

    int read_compute();

    int read_pos_compute();

    int read_neg_compute();

    int get_rule_type(std::string line);


    public: 

    int parse(char* files[], int nfiles);

};

#endif