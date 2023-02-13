#include<stdio.h>
#include<stdlib.h>
#include<pthread.h> 

int mails = 0;
pthread_mutex_t mutex;

void* routine(){
    for (int i = 0; i < 2000000; i++){
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int arcc,char* argv[]){
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    int count_of_thread = 15;
    for (int  i = 0; i < count_of_thread; i++)
    {
        if (pthread_create(th + i, NULL, &routine, NULL) != 0){
            perror("Failed to create thread \n");
            return 1;
        }
        printf("Thread started %d \n", i);
    }
    for (int i = 0; i < count_of_thread; i++)
    {
        if (pthread_join(th[i],NULL)){
            perror("Failed phtread join \n");
            return 2;
        }
        printf("Thread ended %d \n", i);
    }
    
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d \n", mails);
    return 0;
}