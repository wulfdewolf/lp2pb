#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "translate.h"

using namespace std;

// RULE CATEGORIES (smodels)
enum rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};

// PARSER CLASS
class Parser {

    private:

    int parse_file(char* file, Translator &translator);

    void parse_rules(ifstream &infile, Translator &translator);

    void parse_symbol_table(ifstream &infile, char symbol_table[], int highest);

    int parse_compute(ifstream &infile, bool values[]);

    public: 

    int parse(char* files[], int nfiles, Translator &translator);

};

#endif