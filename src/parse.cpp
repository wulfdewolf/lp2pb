/*
*
*   PARSER: handles the parsing of the given input, line by line
*
*
*   --> HIERARCHY:
*        - parse
*           + parse_rules
*             > translator object calls
*           + parse_symbol_table
              > translator object calls
*           + parse_compute
*             > translator object calls
*   
*/
#include "../include/parse.h"

void Parser::parse(string inputfile) {

    // Choose correct input
    istream *in = &cin;
    ifstream infile;
    if(inputfile != "pipe") {
        infile.open(inputfile, ifstream::in);
        if(!infile.is_open()) throw runtime_error("Couldn't open selected inputfile!");
		in = &infile;
    }

    // Parse rules
    parse_rules(*in);

    // Parse symbol table
    parse_symbol_table(*in);

    // Parse compute statements
    parse_compute(*in);

    // Merge SAT and translated constraints
    this->translator->merge();

    infile.close();
    return;
}

// Parses the rules
void Parser::parse_rules(istream& in) {

    int curr;
    string line;
    stringstream temp;

    // Parse first time to get highest variable number and pass simple stuff to lp2sat
    while(getline(in, line)) {
        istringstream iss(line);
        iss>>curr;

        if(curr < 0 || curr > 6) throw runtime_error("Unknow rule type in input file!");
        else if(curr == ZERO_RULE) break;
        // Translate basic, choice and minimize rules immediately
        else if(curr == BASIC || curr == CHOICE) this->translator->translate_sat(line);
        else if(curr == MINIMIZE) this->translator->translate_minimize(iss);
        // Store the rest for later translation --> when highest variable number is known
        else temp << line << '\n';

        // Only read the literal parts for the highest number, skip minimize rules
        switch(curr) {
            case MINIMIZE: 
                continue;
            case WEIGHT: 
                skip(3, iss);
                break;
            case CHOICE:
                // Also read the heads
                iss>>curr;
                parse_max(curr, iss);
            default:
                skip(2, iss);
        }
        parse_max(curr, iss);
    }

    // Parse stored rules to translate now highest is known
    while(getline(temp, line)) {
        istringstream iss(line);
        iss>>curr;

        if(curr == WEIGHT) this->translator->translate_weight(iss);
        else this->translator->translate_constraint(iss);
    }
    return;
}

// Parses the symbol table
void Parser::parse_symbol_table(istream& in) {

    // Add dividor zero to the SAT stream
    this->translator->to_sat << "0" << '\n';

    int curr;
    char curr_symbol;
    bool was_hidden = false;
    string line;

    for(int i = 0; ; i++){

        if(!was_hidden){
            if(getline(in, line)) {
                istringstream iss(line);
                iss>>curr;
                iss>>curr_symbol;
                if(curr == ZERO_RULE) break;
                // Add the line to the SAT stream
                this->translator->to_sat << line << '\n';
            } else break;
        }
    }
}

// Parses the compute statements
void Parser::parse_compute(istream& in) {

    // Add the zero to the SAT stream
    this->translator->to_sat << "0" << '\n';

    int curr;
    string line;
    
    // Skip the 'B+'
    getline(in, line);
    // Add it to the SAT stream
    this->translator->to_sat << "B+" << '\n';

    // Translate the positives
    while(getline(in, line)) {
        this->translator->to_sat << line << '\n';
        istringstream iss(line);
        iss>>curr;

        if(curr == ZERO_RULE) break;
        else this->translator->translate_value(curr, true);
    }

    // Skip the 'B-'
    getline(in, line);
    // Add it to the SAT stream
    this->translator->to_sat << "B-" << '\n';

    // Translate the negatives
    while(getline(in, line)) {
        this->translator->to_sat << line << '\n';
        istringstream iss(line);
        iss>>curr;

        if(curr == ZERO_RULE) break;
        else this->translator->translate_value(curr, false);
    }

    // Get the amount of models
    getline(in, line);
    istringstream iss(line);
    iss>>this->translator->amount_of_models;
    this->translator->to_sat << this->translator->amount_of_models << '\n';

    return;
}

//-----------------------------------------------------------------------------
//                                  UTILITY
//-----------------------------------------------------------------------------
void Parser::parse_max(int amount, istringstream &iss) {
    int curr;
    for(int i = 0; i < amount; i++) {
        iss>>curr;
        this->translator->highest = max(this->translator->highest, curr);
    }
    return;
}