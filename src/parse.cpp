/*
*   Parsing input files
*/
#include "../include/parse.h"

// Parses each file in a given array of filenames
int Parser::parse(char* files[], int nfiles, Translator translator) {

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
int Parser::parse_file(char* file, Translator translator) {

    // Create stream from given filename
    ifstream infile(file);
    
    // Parse the rules
    parse_rules(infile, translator);
    int highest = 3; //translator::highest;

    // Parse symbol table
    char symboltable[highest];
    parse_symbol_table(infile, symboltable, highest);
  
    // Print symbol table (DEBUG)
    for(int i = 0; i < highest; i++) {
        cout << "idx: " << i+1 << " has symbol: " << symboltable[i] << '\n';
    }

    // Parse compute statements
    bool values[highest] = { false };
    int amount_of_models = parse_compute(infile, values);

    // Print svalues (DEBUG)
    for(int i = 0; i < highest; i++) {
        cout << "idx: " << i+1 << " has value: " << values[i] << '\n';
    }
    cout << "Amount of models to calculate: " << amount_of_models << '\n';

    // Close the file
    infile.close();
    return 0;
}

// Parses the rules
void Parser::parse_rules(ifstream &infile, Translator translator) {

    int curr;
    string line;

    // Specific rule parts
    int head;
    int literals_body;
    int negative_body;

    while(getline(infile, line)) {
        istringstream iss(line);

        iss>>curr;
        cout << "Curr rule type= " << curr << '\n';
        if(curr == ZERO_RULE) return;
        
        /*switch(curr) {
            case ZERO_RULE: 
                return;
            case BASIC:
                translator::translate_basic(line);
            case CONSTRAINT:
                translator::translate_constraint(line);
            case CHOICE:
                translator::translate_choice(line);
            case WEIGHT:
                translator::translate_weight(line);
            case MINIMIZE:
                translator::translate_min_max(line);
        }*/
    }
    return;
}

// Parses the symbol table
void Parser::parse_symbol_table(ifstream &infile, char symbol_table[], int highest) {

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
            symbol_table[i] = curr_symbol;
            was_hidden = false;
        } else {
            symbol_table[i] = '/';
            was_hidden = true;
        }
    }
}

// Parses the compute statements
int Parser::parse_compute(ifstream &infile, bool values[]) {

    int curr;
    string line;

    // Skip the 'B+'
    getline(infile, line);

    // Get the positives
    while(getline(infile, line)) {
        istringstream iss(line);
        iss>>curr;
        if(curr == ZERO_RULE) {
            break;
        } else {
            values[curr-1] = true;
        }
    }

    // Skip the negatives part
    while(getline(infile, line)) {
        istringstream iss(line);
        if(curr == ZERO_RULE) {
            break;
        }
    }

    // Get the amount of models to be calculated
    getline(infile, line);
    istringstream iss(line);
    iss>>curr;
    return curr;
}