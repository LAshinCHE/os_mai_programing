#include "lib2.h"
#include "lib1.h"
#include <stdio.h>
#include <assert.h>


void print_menue(){
 printf("________________________YOUR MENU________________________\n");
  printf("          SELECT A FUNCTION TO INTERACT WITH PROGRAM          \n");
  printf("                                                              \n");
  printf("______________________________________________________________\n");
  printf("1 -- to area counting section                                 \n");
  printf("______________________________________________________________\n");
  printf("2 -- to gcf counting section                                  \n");
  printf("______________________________________________________________\n");
  printf("q -- EXIT                                                     \n");
  printf("______________________________________________________________\n");
}


int main(){
    char comand;
    print_menue();
    while((comand=getchar()) != EOF && comand != 'q'){
        switch (comand){
        case '1':
            float f1,f2;
            printf("Type 2 float arguments for Area funcrion\n");
            assert(scanf("%f %f",&f1, &f2) == 2);
            printf("%f rectangle area \n",Area(f1,f2));
            break;
        case '2':
            int i1,i2;
            printf("Type 2 integer arguments for gcf funcrion\n");
            assert(scanf("%d %d",&i1,&i2) == 2);
            printf("%d naive gcf alg \n",GCF(i1,i2));
            break;
        }
    }
    return 0;
}