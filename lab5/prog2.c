#include "lib1.h"
#include "lib2.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

void print_menue(){
 printf("________________________YOUR MENU________________________\n");
  printf("          SELECT A FUNCTION TO INTERACT WITH PROGRAM          \n");
  printf("                                                              \n");
  printf("______________________________________________________________\n");
  printf("0 -- to change type of funcrion u want to use                 \n");
  printf("______________________________________________________________\n");
  printf("1 -- to area counting section                                 \n");
  printf("______________________________________________________________\n");
  printf("2 -- to gcf counting section                                  \n");
  printf("______________________________________________________________\n");
  printf("q -- EXIT                                                     \n");
  printf("______________________________________________________________\n");
}

int main(){
    print_menue();
    float (*AREA)(float a, float b);
    int (*GCF)(int a, int b);
    char comand;
    float f1,f2;
    int i1,i2;
    int cur_lib = 0;
    char* libs[] = {"./libd1.so", "./libd2.so"};
    //dlopen - return "handle", that can employ with other dlopen API function
    void* lib_handler = dlopen(libs[cur_lib], RTLD_LAZY);
    if (lib_handler == NULL){
        perror("there are no such lib in env!\n");
        return 2;
    }
    // dlsym return addres where this loaded in shared memory
    AREA = dlsym(lib_handler,"Area");
    if (AREA == NULL){
        perror("AREA function error, this function is not found\n");
        return 3;
    }
    
    GCF= dlsym(lib_handler,"GCF");
    if (GCF == NULL){
        perror("GCF function error, this function is not found \n");
        return 4;
    }
    while((comand=getchar()) != EOF && comand != 'q'){
        switch (comand){
        case '0':
            if(dlclose(lib_handler) != 0){
                perror("close lib error\n");
                return 5;
            }

            cur_lib = 1 - cur_lib;
            lib_handler = dlopen(libs[cur_lib], RTLD_LAZY);
            if (lib_handler == NULL){
                perror("dlopen error\n");
                return 6;
            }
            AREA = dlsym(lib_handler,"Area");
            if (AREA == NULL){
                perror("AREA function error, this function is not found\n");
                return 7;
            }
            GCF= dlsym(lib_handler,"GCF");
            if (GCF == NULL){
                perror("GCF function error, this function is not found \n");
                return 8;
            }
            break;
        case '1':
                printf("Type 2 float arguments\n");
                assert(scanf("%f %f",&f1, &f2) == 2);
                printf("%f area \n",AREA(f1,f2));
                break;
        case '2':
                printf("Type 2 integer arguments\n");
                assert(scanf("%d %d",&i1, &i2) == 2);
                printf("%d  gcf alg \n",GCF(i1,i2));
                break;
        }
    }

    if(dlclose(lib_handler) != 0){
        perror("close lib error\n");
        return 9;
    }

    return 0;
}