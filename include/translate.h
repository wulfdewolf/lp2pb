#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// TRANSLATOR CLASS
class Translator {

    private:


    public: 

    int highest = 0;

    int to_sat(string line);

    void translate_basic(string line);
    void translate_constraint(string line);
    void translate_choice(string line);
    void translate_weight(string line);
    void translate_min_max(string line);

};

#endif