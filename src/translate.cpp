/*
*
*   TRANSLATOR: handles translation
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


    // Commands to execute
    char cmd_normal[] = "./lp2normal/lp2normal-2.27";
    char cmd_sat[] = "./sat/lp2sat-1.24";

    // Output streams
    stringstream output_normal;
    stringstream output_sat;

    this->executor->exec(cmd_normal, this->to_sat, output_normal);
    this->executor->exec(cmd_sat, output_normal, output_sat);

     /* string linee;

    // DEBUG ---------------- print output
    cout << "\n\nOUTPUT: \n\n";
    while(output_sat.rdbuf()->in_avail()) {
      getline(output_sat, linee);
      cout << linee << '\n';
    }
    // DEBUG ---------------- */


    // Translate generated output to the constraints

    string line;
    int curr;

    // Get variable and constraint number
    getline(output_sat, line);
    get_problem_line(output_sat);

    // Now start translating the clauses
    string symbol;
    while(getline(output_sat, line)) {
        istringstream iss(line);
        if(line == "") break;

        while(iss.good()) {
            iss>>curr;

            // Skip comment lines
            if(line[0] == 'c') break;
            // Linebreak when end of rule is reached
            else if(curr == 0) this->constraints << ">= 1;\n";
            else add_single(abs(curr), 1, curr>0, this->constraints);
        }
    }

    // Read into correct output + add minimize and meta lines

    ostream *out = &cout;
    fstream outputfile;
    // If an outputfile-name was provided
    if(this->outputfile != "pipe") {
        // try to open it
        outputfile.open(this->outputfile, std::fstream::in | std::fstream::out | std::fstream::app);
        // create it if it doesn't exist
        if(!outputfile) outputfile.open(this->outputfile, fstream::in | fstream::out | fstream::trunc);
    }

    // Add the meta line
    *out << "* #variable= " << this->highest << " #constraints= " << this->amount_of_constraints << '\n';

    // Add minimize line
    getline(this->minimize, line);
    *out << line << '\n';

    // Add all the constraints
    while(this->constraints.rdbuf()->in_avail() != 0){
        getline(this->constraints, line);
        *out << line << '\n';
    }
    if(this->outputfile != "pipe") outputfile.close();
}

void Translator::translate_value(int index, int sign) {

    string variable = "x";
    if(!sign) {
        variable = "~x";
    }
    this->constraints << variable << index << " >= " << 1 << ";" << '\n';
    this->amount_of_constraints++;

    return;
}

void Translator::translate_sat(string line) {

    // Rules without aggregates are completely translated to sat
    this->to_sat << line << '\n';

    return;
}

void Translator::translate_constraint(istringstream &iss) {

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
    add_constraint(variables, weights, negatives, positives, bound, new_var, bound, 0);
    
    // X = false constraint

    // --> invert all weights
    fill_n(weights, literals, -1);
    add_constraint(variables, weights, negatives, positives, 0-bound+1, new_var, abs(-literals+bound-1), 1);

    return;
}

void Translator::translate_weight(istringstream &iss) {

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
    add_constraint(variables, weights, negatives, positives, bound, new_var, abs(sum_neg_weights-bound), 0);
    
    // X = false constraint

    // --> invert all weights + calculate worst-case negative
    int sum_pos_weights = 0;
    for(int i = 0; i < literals; i++) {
        if(weights[i] >= 0) sum_pos_weights += weights[i];
        weights[i] = -weights[i];
    }
    add_constraint(variables, weights, negatives, positives, 0-bound+1, new_var, abs(-sum_pos_weights+bound-1), 1);

    return;
}

void Translator::translate_minimize(istringstream &iss) {

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
    this->minimize << "min: ";
    add_series(variables, weights, 0, negatives, 0, this->minimize);
    add_series(variables, weights, negatives, positives+1, 1, this->minimize);
    this->minimize << ";\n";

    return;
}


//-----------------------------------------------------------------------------
//                                  UTILITY
//-----------------------------------------------------------------------------
void Translator::get_problem_line(stringstream &iss) {

}

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

void Translator::add_single(int name, int weight, bool sign, stringstream &iss) {

    // Sign
    if(weight < 0) iss << "-";
    else iss << "+";

    // Weight
    iss << abs(weight);
        
    // Symbol
    const char* var_name = " ~x";
    if(sign) var_name = " x";
    iss << var_name << name << " ";

    return;
}

void Translator::add_series(int names[], int weights[], int start, int end, bool sign, stringstream &iss) {
    for(int i = start; i < end; i++) {
        add_single(names[i], weights[i], sign, iss);
    }
    return;
}

void Translator::add_constraint(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign) {

    add_series(variables, weights, 0, negatives, 0);
    add_single(extra, extra_weight, extra_sign, this->constraints);
    add_series(variables, weights, negatives, positives+1, 1);
    this->constraints << ">= " << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}