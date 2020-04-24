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
*             > translator object calls
*           + parse_compute
*             > translator object calls
*           + parse_amount_of_models
*             > translator object calls
*           + translator->merge
*   
*/
#include "../include/parse.h"

void Parser::parse(string inputfile) {

    istream *in = &cin;
    ifstream infile;

    try {
        // Choose correct input
        if(inputfile != "pipe") {
            infile.open(inputfile, ifstream::in);
            if(!infile.is_open()) throw invalid_inputfile_exception();
            in = &infile;
        }

        // Parse rules
        parse_rules(*in);
        
        // Parse symbol table
        parse_symbol_table(*in);

        // Parse compute statements
        parse_compute(*in, 1);
        parse_compute(*in, 0);

        // Parse amount of models
        parse_amount_of_models(*in);

        // Merge SAT and translated constraints
        this->translator->merge();

    } catch(exception& e) {
        cout << "lp2pb: " << e.what() << '\n';
    }
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

        if(curr < 0 || curr > 6) throw invalid_ruletype_exception();
        else if(curr == ZERO_RULE) break;
        // Translate basic, choice and minimize rules immediately + GET HIGHEST
        else if(curr == BASIC) {
            // head
            iss>>curr;
            this->translator->highest = max(curr, this->translator->highest);
            // literals
            iss>>curr;
            // #negatives
            skip(1, iss);
            parse_max(curr, iss);
            this->translator->translate_sat(line);
        } else if(curr == CHOICE) {
            // heads
            iss>>curr;
            parse_max(curr, iss);
            // literals
            iss>>curr;
            // #negatives
            skip(1, iss);
            parse_max(curr, iss);
            this->translator->translate_sat(line);
        } else if(curr == MINIMIZE) {
            // 0
            skip(1, iss);
            // literals
            iss>>curr;
            // #negatives
            skip(1, iss);
            parse_max(curr, iss);
            this->translator->translate_minimize(line);
        } else { 
        // Store the rest for later translation --> when highest variable number is known + GET HIGHEST
            temp << line << '\n';

            // head
            iss>>curr;
            this->translator->highest = max(curr, this->translator->highest);

            if(curr == CONSTRAINT) {
                // literals
                iss>>curr;
                // #negatives | bound
                skip(2, iss);
                parse_max(curr, iss);
            } else {
                // bound
                skip(1, iss);
                // literals
                iss>>curr;
                // #negatives
                skip(1, iss);
                parse_max(curr, iss);
            }
        }
    }

    // Parse stored rules to translate now highest is known
    while(getline(temp, line)) {
        istringstream iss(line);
        iss>>curr;

        if(curr == CONSTRAINT) this->translator->translate_constraint(iss);
        else this->translator->translate_weight(iss);
    }
    return;
}

// Parses the symbol table 
// TODO: parse to create meta-file that allows backtranslation of found models
void Parser::parse_symbol_table(istream& in) {

    // Add separating zero to the SAT stream
    this->translator->to_lp2sat << "0" << '\n';

    int curr;
    string symbol;
    string line;

    while(getline(in, line)) {
        istringstream iss(line);
        iss>>curr;
        iss>>symbol;

        if(curr == ZERO_RULE) break;
        this->translator->symbol_table[curr] = symbol;
    }

    this->translator->translate_symbol_table();

    // Add separating zero to the SAT stream
    this->translator->to_lp2sat << "0" << '\n';
}

void Parser::parse_compute(istream & in, bool sign) {

    int curr;
    string line;
    string symbol = (sign? "B+":"B-");

    // Symbol
    getline(in, line);
    if(line != symbol) throw missing_compute_exception(sign);

    // Add it to the SAT stream
    this->translator->to_lp2sat << symbol << '\n';

    // Translate
    while(getline(in, line)) {
        istringstream iss(line);
        iss>>curr;

        this->translator->to_lp2sat << line << '\n';
        if(curr == ZERO_RULE) break;
        else this->translator->translate_value(curr, sign);
    }
}

// Parses the amount of models 
void Parser::parse_amount_of_models(istream& in) {

    string line;

    // Get the amount of models
    getline(in, line);
    istringstream iss(line);
    iss>>this->translator->amount_of_models;
    this->translator->to_lp2sat << this->translator->amount_of_models << '\n';

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