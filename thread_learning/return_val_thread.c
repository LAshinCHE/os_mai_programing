#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

void* roll_dice(){
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;
    printf("thread result: %p\n", result);
    return (void*) result;
}

int main(int argc, char* argv[]){
    int* res;
    srand(time(NULL));
    pthread_t  th;
    if(pthread_create(&th,NULL,roll_dice,NULL) != 0){
        return 1;
    }
    if (pthread_join(th,(void**)&res) != 0 )
    {
        return 2;
    }
    printf("Main result: %p\n", res);
    printf("%d - roll dice val\n",*res); 
    free(res);
    return 0;
}