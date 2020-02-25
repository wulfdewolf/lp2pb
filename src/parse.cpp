/*
*   Parsing input files
*/
#include "../include/parse.h"

int Parser::parse(char* files[], int nfiles) {

    // Parse each file separately
    for(int i=0; i < nfiles; i++) {

        // Print out current file name
        cout << "Current file: " << files[i] << '\n';
        int status = parse_file(files[i]);
    }
    return 0;
}


// Parses the rules of the input file
int Parser::parse_rules(ifstream &infile) {

    int curr;
    int highest = 0;
    string line;

    while(getline(infile, line)) {
        istringstream iss(line);

        iss>>curr;
        cout << "Curr rule type= " << curr << '\n';
        if(curr == 0) {
            break;
        } else {

            while(iss>>curr) {
                cout << curr << '\n';
                highest = max(curr, highest);
            }
        }
    }
    return highest;
}

// Parses the symbol table
void Parser::parse_symbol_table(ifstream &infile, char symbol_table[], int highest) {

    int curr;
    char curr_symbol;
    bool was_hidden = false;
    string line;

    for(int i = 0; i < highest; i++){

        if(!was_hidden){
            if(getline(infile, line)) {
                istringstream iss(line);
                iss>>curr;
                iss>>curr_symbol;
                if(curr == 0) break;
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

// Parses one file, completely
int Parser::parse_file(char* file) {

    // Create stream from given filename
    ifstream infile(file);
    
    // Parse the rules
    int highest = parse_rules(infile);

    // Parse symbol table
    char symboltable[highest];
    parse_symbol_table(infile, symboltable, highest);
  

    for(int i = 0; i < highest; i++) {
        cout << "value: " << i+1 << " has symbol: " << symboltable[i] << '\n';
    }

    // Parse compute statements

    // Close the file
    infile.close();
    return 0;
}