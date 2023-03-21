#include"lib1.h"
// Euclide alg
int GCF(int a, int b){
    while (a != 0 && b !=0){
        if (a > b)
            a = a % b;
        else
            b = b % a;
    }
    return a+b;
}

// rectangle area
float Area(float a,float b){
    float area =  a * b;
    return area;
}