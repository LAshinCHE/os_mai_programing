#include "lib2.h"
#include<stdlib.h>
//Naive alg
int GCF(int a, int b){
    int divisor = 1;
    int res;
    while(divisor <= a && divisor <= b){
        if (a % divisor == 0 && b % divisor == 0)
        {
            res = divisor;
        }
        divisor+=1;
    }
    return res;
}

// triangle area
float Area(float a, float b){
    return a * b * 0.5;
}