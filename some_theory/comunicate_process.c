#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 10

int xs[N]; 

int main(int argc,char *argv[]){
    
    for (int i = 0; i < N; i++){
        xs[i] = i + 1;
    }
    int t;
    pid_t pid = fork();

    if (pid == -1){
        perror("Fork was faild\n");
        return 1;
    }else if (pid == 0){
        //child
        printf("Its child process:\t");
        for (int i = 0; i < N / 2; i++){
            t = xs[i];
            xs[i] = xs[N - i - 1];
            xs[N - i - 1] = t;
        }
        for (int i = 0; i < N; i++){
            printf(" %d",xs[i]);
        }
        printf("\n");
    }else{
        //adult
        printf("Its adult process:\t");
        int status_child;
        if(waitpid(pid,&status_child,0) == -1){
            perror("waitpid is error");
            return 2;
        }
        for (int i = 0; i < N; i++){
            printf(" %d",xs[i]);
        }
        printf("\n");
    }
    
    return 0;
}