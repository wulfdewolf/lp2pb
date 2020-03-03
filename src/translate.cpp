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

void Translator::translate_basic(istringstream &iss) {


    // DEBUG --------------- Just to get the maximum
    bool read;
    int curr;
    while(iss.rdbuf()->in_avail() != 0) {
        iss>>curr;
        this->highest = max(this->highest, curr);
    }
    // DEBUG ---------------

    // TODO: add call to lp2sat and translation of returned SAT

    return;
}

void Translator::translate_constraint(istringstream &iss) {

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

    // TODO: translation code

    return;
}

void Translator::translate_choice(istringstream &iss) {
    
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

void Translator::translate_weight(istringstream &iss) {
        
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

void Translator::translate_min_max(istringstream &iss) {
    
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