#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

float* f_ptr;

int read_filename(char** name,int* len){    
    int new_len = 0;
    int capacity = 1;
    char *new_name = (char*)malloc(capacity*sizeof(char));
    if (new_name == NULL)
        return -1;    
    char c;
    int res_read;
    while ((res_read = read(STDIN_FILENO,&c,sizeof(char))) > 0 && c != '\n'){

        new_name[new_len] = c;
        new_len += 1;
        if (new_len  >= capacity){
            capacity *= 2; 
            new_name = (char *)realloc(new_name,sizeof(char)*capacity);
            if (new_name == NULL)
                return -1;
        }
    }
    new_name[new_len] = '\0';
    if (res_read == -1)
        return -1; 
    free(*name);  
    *name = new_name;    
    *len = new_len;
    if (res_read == 0)
        return EOF;
    return 0;
}

int main(int argc, char* argv[]){
    int alloc_memory_size = sizeof(float) * 1024;
    char *filename = NULL;
    int file_len = 0;
    printf("Write file that u want to execute!\n");

    if(read_filename(&filename ,&file_len) == -1){
        perror("reading file error!\n");
        return 1;   
    }
    // the file that we raed the data
    int file, file_message;
    if ((file  = open(filename, O_RDWR | O_CREAT, 0777))  == -1){
        perror("opening msg error!\n");
        return 2;
    }

    if ((file_message = open("msg.bin",O_RDWR | O_CREAT | O_TRUNC, S_IRWXU))  == -1){
        perror("opening file error!\n");
        return 3;
    }

    if (ftruncate(file_message,alloc_memory_size) == -1){
        perror("ftruncate erorr \n");
        return 4;
    }
    
    pid_t id  = fork();

    if (id == -1){
        perror("fork error");
        return 4;
    }
    else if (id  == 0){
        // child process
        if (dup2(file,STDOUT_FILENO) == -1){
            perror("dup2 file_message error\n");
            return 5;
        }
        if(execl("./child","./child",(char*) NULL)  == -1){
            perror("execl was failed \n");
            return 6;
        }
        
	} 
    else{
        f_ptr = mmap(NULL,alloc_memory_size, 
                     PROT_READ |PROT_WRITE , MAP_SHARED,
                     file_message, 0);
        f_ptr[0] = 0;
        int i = 0;
        while (1) {
            if (f_ptr[0] == 1) {
                printf("%d ", f_ptr[(int)f_ptr[1]]);
                f_ptr[0] = 0;
                i++;
            } 
            if (f_ptr[0] == 2) {
                break;
            }
    }
    return 0;
}