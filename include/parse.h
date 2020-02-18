#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// RULE CATEGORIES (smodels)
enum class rule_type{ ZERO_RULE, BASIC, CONSTRAINT, CHOICE, TTTTT, WEIGHT, MINIMIZE};


void parse_rule(std::string rule);

rule_type get_rule_type(std::string rule);


#endif