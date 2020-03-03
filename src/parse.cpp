/*
*
*   PARSING: handles the parsing of the given input files
             file by file; line by line
*
*
*   --> HIERARCHY:
*        - parse
*         * parse_file
*           + parse_rules
*             > translator object calls
*           + parse_symbol_table
*           + parse_compute
*             > translator object calls
*   
*/
#include "../include/parse.h"

// Parses each file in a given array of filenames
int Parser::parse(char* files[], int nfiles, Translator &translator) {

    int status; 

    // Parse each file separately
    for(int i=0; i < nfiles; i++) {

        // Print out current file name
        cout << "Current file: " << files[i] << '\n';
        status = parse_file(files[i], translator);
    }
    return status;
}

// Parses one file completely
int Parser::parse_file(char* file, Translator &translator) {

    // Create stream from given filename
    ifstream infile(file);
    
    // Parse the rules a first time just to get the highest variable number
    parse_rules(infile, translator);

    // Parse symbol table
    parse_symbol_table(infile, translator);

    // Parse compute statements
    parse_compute(infile, translator);
    translator.translate_values();

    // Close the file
    infile.close();
    return 0;
}

// Parses the rules
void Parser::parse_rules(ifstream &infile, Translator &translator) {

    int curr;
    string line;

    while(getline(infile, line)) {
        istringstream iss(line);

        iss>>curr;
        cout << "Curr rule type= " << curr << '\n';
        
        switch(curr) {
            case ZERO_RULE: 
                return;
            case BASIC:
                translator.translate_basic(iss, line);
                break;
            case CONSTRAINT:
                translator.translate_constraint(iss);
                break;
            case CHOICE:
                translator.translate_choice(iss);
                break;
            case WEIGHT:
                translator.translate_weight(iss);
                break;
            case MINIMIZE:
                translator.translate_min_max(iss);
                break;
        }
    }
    return;
}

// Parses the symbol table
void Parser::parse_symbol_table(ifstream &infile, Translator &translator) {

    // Initialise the symbol table
    translator.symbol_table = new char[translator.highest];

    int curr;
    char curr_symbol;
    bool was_hidden = false;
    string line;

    for(int i = 0; ; i++){

        if(!was_hidden){
            if(getline(infile, line)) {
                istringstream iss(line);
                iss>>curr;
                iss>>curr_symbol;
                if(curr == ZERO_RULE) break;
            } else break;
        }

        if(curr == i + 1) {
            translator.symbol_table[i] = curr_symbol;
            was_hidden = false;
        } else {
            translator.symbol_table[i] = '/';
            was_hidden = true;
        }
    }
}

// Parses the compute statements
void Parser::parse_compute(ifstream &infile, Translator &translator) {

    // Initialise the values array
    translator.values = new int[translator.highest];
    for(int i = 0; i < translator.highest; i++) {
        translator.values[i] = 2;
    }

    int curr;
    string line;
    
    // Skip the 'B+'
    getline(infile, line);

    // Get the positives
    while(getline(infile, line)) {
        istringstream iss(line);
        iss>>curr;

        if(curr == ZERO_RULE) break;
        else translator.values[curr-1] = true;
    }

    // Skip the 'B-'
    getline(infile, line);

    // Get the negatives
    while(getline(infile, line)) {
        istringstream iss(line);
        iss>>curr;

        if(curr == ZERO_RULE) break;
        else translator.values[curr-1] = false;
    }

    // Get the amount of models
    getline(infile, line);
    istringstream iss(line);
    iss>>translator.amount_of_models;

    return;
}