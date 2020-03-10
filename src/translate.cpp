/*
*
*   TRANSLATOR: handles translation of the different rule types
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

    // Command to execute
    char cmd[16] = "sat/lp2sat-1.24";

    // DEBUG ---------------- will be to_sat
    ifstream infile("test");
    // DEBUG ----------------

    // Output written to stringstream
    stringstream output;

    //this->executor->exec(cmd, infile, output);

    // DEBUG ---------------- print output
    string linee;
    while(output.good()) {
      getline(output, linee);
      cout << linee << '\n';
    }
    // DEBUG ---------------- 
}

void Translator::translate_sat() {

}

void Translator::translate_values() {

    // Loop over all parsed values and add the corresponding constraints
    for(int i = 0; i < highest; i++) {

        switch(this->values[i]) {            
            case 0: 
                this->constraints << "~x" << i << " >= " << 1 << ";" << '\n';
                this->amount_of_constraints++;
                break;
            case 1: 
                this->constraints << 'x' << i << " >= " << 1 << ";" << '\n';
                this->amount_of_constraints++;
                break;
            case 2:
                break;
        }
    }

    // DEBUG --------------- Display current constraints
    cout << "IN CONSTRAINTS: \n\n";
    while(this->constraints.rdbuf()->in_avail() != 0){
        string line;
        getline(this->constraints, line);
        cout << line << '\n';
    }
    // DEBUG ---------------

    // DEBUG --------------- Display current to_sat
    cout << "\n\nIN SAT: \n\n";
    while(this->to_sat.rdbuf()->in_avail() != 0){
        string line;
        getline(this->to_sat, line);
        cout << line << '\n';
    }
    // DEBUG ---------------
}

void Translator::translate_basic(istringstream &iss, string line) {

    // Add correct parts to streams
    this->to_sat << line << '\n';

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
    int neg[negatives+1];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);


    //------ Add correct parts to streams

    //-- to_sat
    int new_var = ++this->highest;

    this->to_sat << 1 << " " << head << " " << 1 << " " << 0 << " " << new_var << '\n';
    this->to_sat << 3 << " " << 1 << " " << new_var << " " << 0 << " " << 0 << " " << '\n';

    //-- constraints
    int neg_weights[negatives+1];
    int pos_weights[positives];
    fill_n(neg_weights, negatives, 1);
    fill_n(pos_weights, positives, 1);
    neg_weights[negatives] = bound;
    neg[negatives] = head;

    // X = true constraint
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, bound);
    
    // X = false constraint
    fill_n(neg_weights, negatives, -1);
    fill_n(pos_weights, positives, -1);
    neg_weights[negatives] = bound + literals;
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, 0-bound);

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
    }
}

void Translator::add_constraint(int neg[], int neg_weights[], int negatives, int pos[], int pos_weights[], int positives, int value) {

    // Negatives
    for(int i = 0; i < negatives; i++) {

        // Sign
        if(neg_weights[i] < 0) this->constraints << "- ";
        else this->constraints << "+ ";

        // Weight
        this->constraints << abs(neg_weights[i]);
        
        // Symbol
        this->constraints << " ~x" << neg[i] << " ";
    }

    // Positives
    for(int i = 0; i < positives; i++) {

        // Sign
        if(pos_weights[i] < 0) this->constraints << "- ";
        else this->constraints << "+ ";

        // Weight
        this->constraints << abs(pos_weights[i]);
        
        // Symbol
        this->constraints << " x" << pos[i] << " ";
    }
    this->constraints << ">=" << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}