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

    // Parse hierarchy
    void parse_file(char* file, Translator &translator);
    void parse_rules(ifstream &infile, Translator &translator);
    void parse_symbol_table(ifstream &infile, Translator &translator);
    void parse_compute(ifstream &infile, Translator &translator);


    public: 

    void parse(char* files[], int nfiles, Translator &translator);

};

#endif