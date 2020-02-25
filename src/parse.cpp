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

// Parses one file, completely
int Parser::parse_file(char* file) {

    ifstream infile(file);
    string line;
    
    int curr;
    int highest;

    // First parse all rules
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

    // Parse symbol table
    char curr_symbol;
    char symboltable[highest];
    bool was_hidden = false;

    for(int i = 0; i < highest; i++){

        if(!was_hidden){
            if(getline(infile, line)) {
                istringstream iss(line);
                iss>>curr;
                iss>>curr_symbol;
            } else break;
        }

        if(curr == 0){
            break;
        } else if(curr == i + 1) {
            symboltable[i] = curr_symbol;
            was_hidden = false;
        } else {
            symboltable[i] = '/';
            was_hidden = true;
        }
    }

    for(int i = 0; i < highest; i++) {
        cout << "value: " << i+1 << " has symbol: " << symboltable[i] << '\n';
    }

    // Parse compute statements

    // Close the file
    infile.close();
    return 0;
}