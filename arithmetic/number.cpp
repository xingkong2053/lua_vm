//
// Created by Administrator on 2024/4/25.
//


#include "number.h"
#include <cmath>

int iFloorDiv(int a, int b){
    if((a> 0 && b >0) || (a < 0 && b < 0) || a% b == 0){
        return a/b;
    } else {
        return a/b - 1;
    }
}

double dFloorDiv(double a, double b) {
    return floor(a/b);
}

int iMod(int a, int b) {
    return a - iFloorDiv(a, b) * b;
}

double dMod(double a, double b) {
    return a - dFloorDiv(a, b) * b;
}
