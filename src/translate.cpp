/*
*
*   TRANSLATOR
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

void Translator::translate_value(int index, int sign) {

    switch(sign) {            
        case 0: 
            this->constraints << "~x" << index << " >= " << 1 << ";" << '\n';
            this->amount_of_constraints++;
            break;
        case 1: 
            this->constraints << 'x' << index << " >= " << 1 << ";" << '\n';
            this->amount_of_constraints++;
            break;
        case 2:
            break;
    }
}

void Translator::translate_sat(istringstream &iss, string line) {

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

    // Read the arrays of variables
    int variables[literals];
    int weights[literals];
    read_literals(variables, literals, iss);

    /*
    *   Add correct parts to streams
    */ 

    // -- To_sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);

    // -- Constraints

    // X = true constraint

    // All weights are 1 here so we don't need to calculate the sum
    fill_n(weights, literals, 1);
    add_constraint_with_extra(variables, weights, negatives, positives, bound, new_var, bound, 0);
    
    // X = false constraint

    // --> invert all weights
    fill_n(weights, literals, -1);
    add_constraint_with_extra(variables, weights, negatives, positives, 0-bound+1, new_var, abs(-literals+bound-1), 1);

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

    // Read the arrays of variables + their weights
    int variables[literals];
    int weights[literals];
    read_literals(variables, literals, iss);
    read_literals(weights, literals, iss);

    /*
    *   Add correct parts to streams
    */ 

    // -- To_sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);


    // -- Constraints

    // X = true constraint

    // calculate worst-case negative
    int sum_neg_weights = 0;
    for(int i = 0; i < literals; i++) {
        if(weights[i] < 0) sum_neg_weights += weights[i];
    }
    add_constraint_with_extra(variables, weights, negatives, positives, bound, new_var, abs(sum_neg_weights-bound), 0);
    
    // X = false constraint

    // --> invert all weights + calculate worst-case negative
    int sum_pos_weights = 0;
    for(int i = 0; i < literals; i++) {
        if(weights[i] >= 0) sum_pos_weights += weights[i];
        weights[i] = -weights[i];
    }
    add_constraint_with_extra(variables, weights, negatives, positives, 0-bound+1, new_var, abs(-sum_pos_weights+bound-1), 1);

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
    int variables[literals];
    int weights[literals];
    read_literals(variables, literals, iss);
    read_literals(weights, literals, iss);

    /*
    *   Add correct parts to streams
    */ 
    this->constraints << "min: ";
    add_series(variables, weights, 0, negatives, 0);
    add_series(variables, weights, negatives, positives+1, 1);
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

void Translator::add_single(int name, int weight, bool sign) {

    // Sign
    if(weight < 0) this->constraints << "-";
    else this->constraints << "+";

    // Weight
    this->constraints << abs(weight);
        
    // Symbol
    const char* var_name = " ~x";
    if(sign) var_name = " x";
    this->constraints << var_name << name << " ";

    return;
}

void Translator::add_series(int names[], int weights[], int start, int end, bool sign) {
    for(int i = start; i < end; i++) {
        add_single(names[i], weights[i], sign);
    }
    return;
}

void Translator::add_constraint_with_extra(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign) {

    add_series(variables, weights, 0, negatives, 0);
    add_single(extra, extra_weight, extra_sign);
    add_series(variables, weights, negatives, positives+1, 1);
    this->constraints << ">= " << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}

void Translator::add_constraint(int variables[], int weights[], int negatives, int positives, int value) {

    add_series(variables, weights, 0, negatives, 0);
    add_series(variables, weights, negatives, positives+1, 1);
    this->constraints << ">= " << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}