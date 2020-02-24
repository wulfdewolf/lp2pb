/*
*   Parsing input files, create rule streams 
*
*
*
*/
#include "../include/parse.h"

int Parser::parse(char* files[], int nfiles) {

        for(int i=0; i < nfiles; i++) {

            // Print out current file
            cout << "Current file: " << files[i] << endl;

            std::ifstream infile(files[i]);
            std::string line;

            int curr;

            while(std::getline(infile, line)) {
                std::istringstream iss(line);
                while(iss>>curr) {
                    cout << curr << endl;
                }
            }
        }
            return 0;
}