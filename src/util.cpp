/*
*
*   UTILITY
*   
*/

#include "../include/util.h"

void skip(int amount, istringstream &iss) {
    int curr;
    for(int i = 0; i < amount; i++) {
        iss>>curr;
    }
    return;
}