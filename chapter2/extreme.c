#include "extreme.h"

double avg(int* array, int lenght, average_type_t type) {
    // COMPUTES NORMAL AND SQUARED AVERAGE
    
    if(lenght == 0 || type == NONE) {
        return 0;
    }

    double sum = 0.0;
    for(int i =0; i < lenght; i++){
        if(type == NORMAL) {
            sum += array[i];
        } 
        else if (type == SQUARED) {
            sum += array[i] * array[i];
        }
    }

    return sum / lenght;
}