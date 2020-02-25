#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// RULE CATEGORIES (smodels)
enum rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};

// PARSER CLASS
class Parser {

    private:

    int amount_of_variables = 0;
    int amount_of_contraints = 0;

    int parse_file(char* file);

    int parse_rules(ifstream &infile);

    void parse_symbol_table(ifstream &infile, char symbol_table[], int highest);

    int parse_compute(ifstream &infile);

    public: 

    int parse(char* files[], int nfiles);

};

#endif