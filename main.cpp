/*
*
*   lp2pb: ASP to PB translator
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

    if(argc !=  4 && argc != 2 && argc != 1) {
        cout << "Usage: ./lp2pb <inputfile1> <-o outputfile>" << endl;

    } else {

        Executor executor;
        Translator translator;
        Parser parser;

        parser.translator = &translator;
        translator.executor = &executor;

        string inputfile = "pipe";
    
        if(argc == 4) {
            if(cmdOptionExists(argv+1, argv+argc, "-o")) {
                translator.outputfile = getCmdOption(argv+1, argv+argc, "-o");
                inputfile = argv[1];
            } else cout << "Usage: ./lp2pb <inputfile1> <-o outputfile>" << endl;
        } else if(argc == 2) {
            inputfile = argv[1];
        }

        parser.parse(inputfile);
    }
    return 0;
}