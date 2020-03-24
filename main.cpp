/*
*
*   lp2pb: ASP to PB translator
* 
*/
#include "include/parse.h"
#include "include/translate.h"
#include "include/util.h"
using namespace std;


int main(int argc, char* argv[]) {

    string usage_message = "Usage: ./lp2pb <-i inputfile1> <-o outputfile>\n";

    if(argc ==  5 || argc == 3 || argc == 1) {

        string inputfile = "pipe";
        Executor executor;
        Translator translator;
        Parser parser;

        parser.translator = &translator;
        translator.executor = &executor;

        if(argc == 5) {

            if(cmdOptionExists(argv+1, argv+argc, "-o") && cmdOptionExists(argv+1, argv+argc, "-i")) {
                translator.outputfile = getCmdOption(argv+1, argv+argc, "-o");
                inputfile = getCmdOption(argv+1, argv+argc, "-i");
            } else {
                cout << usage_message;
                return 0;
            }

        } else if(argc == 3) {

            if(cmdOptionExists(argv+1, argv+argc, "-o")) translator.outputfile = getCmdOption(argv+1, argv+argc, "-o");
            else if(cmdOptionExists(argv+1, argv+argc, "-i")) inputfile = getCmdOption(argv+1, argv+argc, "-i");
            else {
                cout << usage_message;
                return 0;
            }
        }

        // Start parsing
        parser.parse(inputfile);
    } else cout << usage_message;
    return 0;
}