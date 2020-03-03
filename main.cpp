/*
*
*   lp2pb: ASP to PB translator
*
*   author: Wolf De Wulf
* 
*/
#include <iostream>
#include <algorithm>
#include "include/parse.h"
#include "include/translate.h"
using namespace std;

char* getCmdOption(char ** begin, char ** end, const std::string & option) {
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Usage: ./lp2pb inputfile1 inputfile2 ..." << endl;

    } else {

        Parser parser;
        Translator translator;

        int test = parser.parse(argv+1, argc-1, translator);
    }

    return 0;
}