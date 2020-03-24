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

// EXCEPTIONS
struct invalid_inputfile_exception : public exception {
    const char* what() const throw() {
        return "Inputfile doesn't exist or couldn't be opened!";
    }
};

struct invalid_ruletype_exception : public exception {
    const char* what() const throw() {
        return "Invalid ruletype in input!";
    }
};

struct missing_compute_exception : public exception {

    string m_msg;

    missing_compute_exception(bool sign)
    : m_msg(string("Missing ") + string(sign? "positive " : "negative ") + string("compute statement!"))
    {}

    const char* what() const throw() {
        return m_msg.c_str();
    }
};

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