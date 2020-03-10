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

    // basic rules are completely translated to sat
    this->to_sat << line << '\n';

    return;
}

void Translator::translate_constraint(istringstream &iss, string line) {

    /*
    *   Get rule data
    */ 
    int head, literals, negatives, positives, bound;
    iss>>head;
    iss>>literals;
    iss>>negatives;
    positives = literals - negatives;
    iss>>bound;

    // Read the arrays of positives and negatives
    int neg[negatives+1];
    int pos[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);

    /*
    *   Add correct parts to streams
    */ 

    // to_sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);

    // constraints
    int neg_weights[negatives+1];
    int pos_weights[positives];
    fill_n(neg_weights, negatives, 1);
    fill_n(pos_weights, positives, 1);
    neg_weights[negatives] = bound;
    neg[negatives] = head;

    // X = true constraint
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, bound);
    
    // X = false constraint
    int sum_weights = 0;
    for(int i = 0; i < negatives; i++) {
        sum_weights += neg_weights[i];
        neg_weights[i] = -1;
    }
    for(int i = 0; i < positives; i++) {
        sum_weights += pos_weights[i];
        pos_weights[i] = -1;
    }
    neg_weights[negatives] = abs(sum_weights - bound) + 1;
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, 0-bound+1);

    return;
}

void Translator::translate_choice(istringstream &iss, string line) {

    // choice rules are completely translated to sat
    this->to_sat << line << '\n';

    return;
}

void Translator::translate_zero(istringstream &iss, string line) {
    return;
}

void Translator::translate_weight(istringstream &iss, string line) {

    /*
    *   Get rule data
    */ 
    int head, bound, literals, negatives, positives;
    iss>>head;
    iss>>bound;
    iss>>literals;
    iss>>negatives;
    positives = literals - negatives;

    // Read the arrays of positives and negatives + their weights
    int neg[negatives+1];
    int neg_weights[negatives+1];
    int pos[positives];
    int pos_weights[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);
    read_literals(neg_weights, negatives, iss);
    read_literals(pos_weights, positives, iss);

    /*
    *   Add correct parts to streams
    */ 

    // to_sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);

    // constraints
    neg_weights[negatives] = bound;
    neg[negatives] = head;

    // X = true constraint
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, bound);
    
    // X = false constraint

    // --> invert all weights
    int sum_weights = 0;
    for(int i = 0; i < positives; i++) {
        sum_weights += abs(pos_weights[i]);
        pos_weights[i] = -pos_weights[i];
    }
    for(int i = 0; i < negatives; i++) {
        sum_weights += abs(neg_weights[i]);
        neg_weights[i] = -neg_weights[i];
    }
    neg_weights[negatives] = abs(sum_weights - bound) + 1;
    add_constraint(neg, neg_weights, negatives+1, pos, pos_weights, positives, 0-bound+1);

    return;
}

void Translator::translate_min_max(istringstream &iss, string line) {

    /*
    *   Get rule data
    */ 
    int literals, negatives, positives;
    iss>>literals; // skip the zero
    iss>>literals;
    iss>>negatives;
    positives = literals - negatives;

    // Read the arrays of positives and negatives + their weights
    int neg[negatives];
    int neg_weights[negatives];
    int pos[positives];
    int pos_weights[positives];

    read_literals(neg, negatives, iss);
    read_literals(pos, positives, iss);
    read_literals(neg_weights, negatives, iss);
    read_literals(pos_weights, positives, iss);

    /*
    *   Add correct parts to streams
    */ 
    this->constraints << "min: ";
    add_series(neg, neg_weights, negatives, 0);
    add_series(pos, pos_weights, positives, 1);
    this->constraints << ";\n";

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

void Translator::add_sat(int rule[], int amount) {

    int i = 0;
    while(i < amount) {
        this->to_sat << rule[i];
        if(++i == amount) {
            this->to_sat << '\n';
            break;
        } else this->to_sat << " ";
    }
}

void Translator::add_series(int names[], int weights[], int amount, bool sign) {
    for(int i = 0; i < amount; i++) {

        // Sign
        if(weights[i] < 0) this->constraints << "-";
        else this->constraints << "+";

        // Weight
        this->constraints << abs(weights[i]);
        
        // Symbol
        const char* var_name = " ~x";
        if(sign) var_name = " x";
        this->constraints << var_name << names[i] << " ";
    }
}

void Translator::add_constraint(int neg[], int neg_weights[], int negatives, int pos[], int pos_weights[], int positives, int value) {

    add_series(neg, neg_weights, negatives, 0);
    add_series(pos, pos_weights, positives, 1);
    this->constraints << ">= " << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}