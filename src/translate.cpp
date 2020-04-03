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
    char cmd_normal[] = "lp2normal";
    char option_normal [] = "-e";
    char* argv_normal[] = { cmd_normal, option_normal, NULL};
    char cmd_lp[] = "lp2lp2";
    char* argv_lp[] = { cmd_lp, NULL};
    char cmd_sat[] = "lp2sat";
    char* argv_sat[] = { cmd_sat, NULL};

    // Output streams
    stringstream output_normal;
    stringstream output_lp;
    stringstream output_sat;
    if(this->executor->exec(argv_normal, this->to_lp2sat, output_normal) != 0) throw executable_exception();
    if(this->executor->exec(argv_lp, output_normal, output_lp) != 0) throw executable_exception();
    if(this->executor->exec(argv_sat, output_lp, output_sat) != 0) throw executable_exception();

    string line;
    int curr;

    // Now start translating the clauses
    string symbol;
    while(getline(output_sat, line)) {
        istringstream iss(line);
        if(line == "") break;

        while(iss.good()) {
            iss>>curr;

            // Skip comment lines
            if(line[0] == 'c' || line[0] == 'p') break;
            // Linebreak when end of rule is reached
            else if(curr == 0) {
                this->constraints << ">= 1;\n";
                this->amount_of_constraints++;
            } else {
                this->highest = max(this->highest, abs(curr));
                add_single(abs(curr), 1, curr>0, this->constraints);
            }
        }
    }

    // Read into correct output + add minimize and meta lines
    ostream *out = &cout;
    fstream outputfile;
    // If an outputfile-name was provided
    if(this->outputfile != "pipe") {
        // Check if it exists already
        if(file_exists(this->outputfile.c_str())) {
            cout << "Chosen outputfile exists already, append to it? (y/n)\n";

            get_input:
            char response;
            cin >> response;
                switch(response) {
                    case 'y': break;
                    case 'n':
                        throw invalid_outputfile_exception();
                        break;
                    default: 
                        cout << "Invalid choice!\n";
                        goto get_input;
                }
        }
        outputfile.open(this->outputfile, std::fstream::in | std::fstream::out | std::fstream::app);
        out = &outputfile;
    }

    // Add the meta line
    *out << "* #variable= " << this->highest << " #constraint= " << this->amount_of_constraints << '\n';

    // Add minimize line
    getline(this->minimize, line);
    if(line != "") *out << line << ";" << '\n';

    // Add all the constraints
    while(this->constraints.rdbuf()->in_avail() != 0){
        getline(this->constraints, line);
        *out << line << '\n';
    }
    outputfile.close();
}

void Translator::translate_symbol_table() {

    // Iterator
    map<int, string>::iterator it;

    // Add all symbols
    for ( it = this->symbol_table.begin(); it != this->symbol_table.end(); it++ ) {
        to_lp2sat << it->first << " " << it->second << "\n";
    }
}

void Translator::translate_value(int index, int sign) {

    this->constraints << "+1 " << (sign? "x":"~x") << index << " >= " << 1 << ";" << '\n';
    this->amount_of_constraints++;

    return;
}

void Translator::translate_sat(string line) {

    // Rules without aggregates are completely translated to sat
    this->to_lp2sat << line << '\n';

    return;
}

void Translator::translate_constraint(istringstream &iss) {

    /*
    *   Get rule data
    */ 
    int head, literals, negatives = 0, positives = 0, bound;
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

    // -- to_lp2sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);
    this->symbol_table[new_var] = "lptopb(" + to_string(new_var) + ")";

    // -- constraints

    // X = true constraint

    // All weights are +1 here so we don't need to calculate the sum
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
    int head, bound, literals, negatives = 0, positives = 0;
    iss>>head;
    iss>>bound;
    iss>>literals;
    iss>>negatives;
    positives = literals - negatives;

    // Read the arrays of variables + their weights
    int variables[literals];
    int weights[literals];
    read_literals(variables, literals, iss);
    read_weights(weights, literals, iss);

    /*
    *   Add correct parts to streams
    */ 

    // -- to_lp2sat
    int new_var = ++this->highest;
    int new_var_rule[5] = { 1, head, 1, 0, new_var };
    int new_choice_rule[5] = { 3, 1, new_var, 0, 0 };

    add_sat(new_var_rule, 5);
    add_sat(new_choice_rule, 5);
    this->symbol_table[new_var] = "lptopb(" + to_string(new_var) + ")";


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
    read_weights(weights, literals, iss);

    // Check if it is the first one --> calculate initial penalty if so
    bool was_first = this->minimize.rdbuf()->in_avail() == 0;
    int penalty = 0;
    if(!was_first) {
        string line; 
        int curr;
        while(this->minimize_weights.rdbuf()->in_avail() != 0) {
            this->minimize_weights>>curr;
            penalty += curr;
        }
    }

    // Add penalty to weights and store
    for(int i=0; i < literals; i++) {
        weights[i] *= penalty+1;
        this->minimize_weights << weights[i] << " ";
    }

    // Also add previous penalty
    this->minimize_weights << penalty;

    /*
    *   Add correct parts to streams
    */ 
    if(was_first) this->minimize << "min: ";
    add_series(variables, weights, 0, negatives, 0, this->minimize);
    add_series(variables, weights, negatives, negatives+positives, 1, this->minimize);

    return;
}


//-----------------------------------------------------------------------------
//                                  UTILITY
//-----------------------------------------------------------------------------
void Translator::read_literals(int array[], int amount, istringstream &iss, bool is_weight) {
    for(int i = 0; i < amount; i++) {
        iss>>array[i];
        if(!is_weight && this->symbol_table.count(array[i]) == 0) this->symbol_table[array[i]] = "lptopb(" + to_string(array[i]) + ")";
    }
}

void Translator::add_sat(int rule[], int amount) {

    int i = 0;
    while(i < amount) {
        this->to_lp2sat << rule[i];
        if(++i == amount) {
            this->to_lp2sat << '\n';
            break;
        } else this->to_lp2sat << " ";
    }
}

void Translator::add_single(int name, int weight, bool sign, stringstream &iss) {

    // Sign
    iss << (weight<0? "-":"+");

    // Weight
    iss << abs(weight);
        
    // Symbol
    iss << (sign? " x":" ~x") << name << " ";

    return;
}

void Translator::add_series(int names[], int weights[], int start, int end, bool sign, stringstream &iss) {
    for(int i = start; i < end; i++) {
        add_single(names[i], weights[i], sign, iss);
    }
    return;
}

void Translator::add_constraint(int variables[], int weights[], int negatives, int positives, int value, int extra, int extra_weight, bool extra_sign) {

    if(negatives) add_series(variables, weights, 0, negatives, 0);
    add_single(extra, extra_weight, extra_sign, this->constraints);
    if(positives) add_series(variables, weights, negatives, negatives+positives, 1);
    this->constraints << ">= " << value << ';' << '\n';

    // Increase counter
    this->amount_of_constraints++;
}

bool Translator::file_exists(const char *fileName)
{
    ifstream infile(fileName);
    return infile.good();
}