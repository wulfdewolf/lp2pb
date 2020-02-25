/*
*   Parsing input files
*/
#include "../include/parse.h"

int Parser::parse(char* files[], int nfiles) {

    // Parse each file separately
    for(int i=0; i < nfiles; i++) {

        // Print out current file name
        cout << "Current file: " << files[i] << endl;
        int status = parse_file(files[i]);
    }
    return 0;
}

int Parser::get_rule_type(std::string line){
    return line[0];
}

int Parser::parse_file(char* file) {

    std::ifstream infile(file);
    std::string line;
    
    int curr;
    int curr_type;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);

        // First parse all the rules
        while(iss>>curr && line != "0") {
            curr_type = get_rule_type(line);
            cout << curr << endl;
            cout << curr_type << endl;
        }

        // Then parse the meta data

        // Parse the symbol table

        // Parse the compute statements
    }
    return 0;
}