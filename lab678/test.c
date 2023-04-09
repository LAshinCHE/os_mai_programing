#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RC.h"

//gcc -Wall server.c  -o prog -L/usr/local/lib -lzmq
//gcc -std=c99 -Wall test.c RC.c -o test -lm

int main()
{
    char str1[20] = "aasdadasdasda";
    char str2[20] = "asa";
    char str3[20] = "baaaaaa";

    long result1 = RC(str1,str2);
    long result2 = RC(str3,str2);

    printf("Searching '%s' in '%s' result = %ld  \n",str2,str1, result1);
    printf("Searching '%s' in '%s' result = %ld  \n",str2,str3, result2);

    return 0;
}