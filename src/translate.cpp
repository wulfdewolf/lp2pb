/*
*
*   TRANSLATION: handles translation of the different rule types
*
*
*   --> some utility methods are implemented at the bottom
*   
*/
#include "../include/translate.h"


//-----------------------------------------------------------------------------
//                              RULE TRANSLATION
//-----------------------------------------------------------------------------
void Translator::merge() {

}

void Translator::translate_sat() {

}

void Translator::translate_values() {

    // Loop over all parsed values and add the corresponding constraints
    for(int i = 0; i < highest; i++) {
        switch(this->values[i]) {
            case 0: 
                this->constraints << 'x' << i << " < " << 1 << " ;" << '\n';
                this->amount_of_constraints++;
                break;
            case 1: 
                this->constraints << 'x' << i << " >= " << 1 << " ;" << '\n';
                this->amount_of_constraints++;
                break;
            case 2:
                break;
        }
    }

    // DEBUG --------------- Display current constraints
    while(this->constraints.rdbuf()->in_avail() != 0){
        string line;
        getline(this->constraints, line);
        cout << line << '\n';
    }
    // DEBUG ---------------
}

void Translator::translate_basic(istringstream &iss, string line) {


    // DEBUG --------------- Just to get the maximum
    bool read;
    int curr;
    while(iss.rdbuf()->in_avail() != 0) {
        iss>>curr;
        this->highest = max(this->highest, curr);
    }
    // DEBUG ---------------
    

    // TODO: translation code

    return;
}

void Translator::translate_constraint(istringstream &iss, string line) {

    // Get rule data
    int head;
    iss>>head;

    int literals;
    iss>>literals;

    int negatives;
    iss>>negatives;

    int positives = literals - negatives;

    int bound;
    iss>>bound;

    // Read the arrays of positives and negatives
    int neg[negatives];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);

    // Add correct parts to streams
    int new_var = ++this->highest;
    this->to_sat << 1 << head << 1 << 0 << new_var << '\n';
    this->constraints << "test" << '\n';
    this->amount_of_constraints++;

    return;
}

void Translator::translate_choice(istringstream &iss, string line) {
    
    // Get rule data
    int amount_of_heads;
    iss>>amount_of_heads;

    int heads[amount_of_heads];
    read_literals(heads, amount_of_heads, iss);

    int literals;
    iss>>literals;

    int negatives;
    iss>>negatives;

    int positives = literals - negatives;

    // Read the arrays of positives and negatives
    int neg[negatives];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);

    // TODO: translation code

    return;
}

void Translator::translate_weight(istringstream &iss, string line) {
        
    // Get rule data
    int head;
    iss>>head;

    int bound;
    iss>>bound;

    int literals;
    iss>>literals;

    int negatives;
    iss>>negatives;

    int positives = literals - negatives;

    // Read the arrays of positives and negatives
    int neg[negatives];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);

    // Read the array of weights
    int weights[literals];
    read_literals(weights, literals, iss);

    // TODO: translation code

    return;
}

void Translator::translate_min_max(istringstream &iss, string line) {
    
    // Get rule data
    int literals;
    iss>>literals; // Skip the zero
    iss>>literals;

    int negatives;
    iss>>negatives;

    int positives = literals - negatives;

    // Read the arrays of positives and negatives
    int neg[negatives];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);

    // Read the array of weights
    int weights[literals];
    read_literals(weights, literals, iss);

    // TODO: translation code

    return;
}


//-----------------------------------------------------------------------------
//                                  UTILITY
//-----------------------------------------------------------------------------
void Translator::read_literals(int array[], int amount, istringstream &iss) {
    for(int i = 0; i < amount; i++) {
        iss>>array[i];
        this->highest = max(this->highest, array[i]);
    }
}

string Translator::exec(string cmd) {
        array<char, 128> buffer;
        string result;
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
}