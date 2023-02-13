#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

pthread_mutex_t mutex;

void* roll_dice(){
    int val =  rand() % 6 + 1;
    int* res = malloc(sizeof(int));
    *res = val;
    return (void*) res;
}

int main(int  argc,char* argv[]){
    srand(time(NULL));
    int thread_count = 5;
    int* dice_val;
    pthread_t th[thread_count]; 
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < thread_count; i++){
        if (pthread_create(th + i, NULL,roll_dice, NULL) != 0){
            perror("Failed to create thread \n");
            return 1;
        }  
    }
    for (int i = 0; i < thread_count; i++){
        if (pthread_join(th[i],(void **) &dice_val) != 0){
            perror("Failed phtread join \n");
            return 2;
        }
        printf("%d - dise val \n", *dice_val);
        
    }
    free(dice_val);
    pthread_mutex_destroy(&mutex);
    return 0;
}