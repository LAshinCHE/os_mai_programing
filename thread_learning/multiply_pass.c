#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include <string.h>

int NUM_THREADS =  100;

typedef struct{
    long long num;
    long long sum1;
    char name[20];
}_args;


void* func(void* args){
    _args* data = (_args *)args;
    printf("num: %lld value: %lld --- NAME:%s \n", data->num,data->sum1,data->name);
    free(data);
}

int main(int argc, char* argv[]){
    pthread_t th[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++){
        _args *a  = malloc(sizeof(_args));
        a->num = i;
        a->sum1 = i+10;
        strcpy(a->name, "GEORGE");
        if(pthread_create(th + i, NULL,&func ,a) != 0){
            perror("Failed to create thread!\n");
            return 1;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++){
        if(pthread_join(th[i], NULL) != 0){
            perror("Failed to join thread!\n");
            return 2;
        }
    }
    return 0;
}