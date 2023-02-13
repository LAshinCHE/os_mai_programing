#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


int THREAD_COUNT = 2;
int arr[10] = { 0, 1, 2, 3, 4, 5, 6, 7,  8, 9 }

void* sum_arr(void* args){
    int index = *(int*)args;

    for (int i = index; i < index +5; i++)
    {
        result += arr[i];
    }
    
    return result;
}

int main(){
    pthread_t th[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        int* index =  (int*)malloc(sizeof(int));
        *index = i;
        if (pthread_create(th + i, NULL,&sum_arr, index) != 0){
            perror("thread creatind is faild!\n");
            return 1;
        }
    }
    
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if(pthread_join(th[i],ans)){
            perror("thread join is faild\n");
            return 2;
        }
    }
    
    
    
    return 0;
}