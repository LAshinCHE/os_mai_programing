#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int read_filename(char** name,int* len){    
    errno = 0;
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
        return errno; 
    free(*name);  
    *name = new_name;    
    *len = new_len;
    if (res_read == 0)
        return EOF;
    return 0;
}

int main(int argc, char* argv[]){
    char *filename = NULL;
    errno = 0;
    int fd[2];
    int file_len = 0;
    printf("Write file that u want to execute!\n");
    if(read_filename(&filename ,&file_len)){
        perror("reading file error!\n");
        return -1;   
    }

    int file  = open(filename, O_RDWR | O_CREAT, 0777);
    if (file  == -1){
        perror("opening file error!\n");
        return 2;
    }
    
    if(pipe(fd) == -1){
        perror("An error ocurred with opening the \n");
        return 1;
    }

    int id  = fork();

    if (id == -1){
        perror("fork error");
        return 9;
    }
    else if (id  == 0){
        if (dup2(file, STDIN_FILENO) == -1){
            perror("dup2 error");
            return 10;
        }
		else if (dup2(fd[1], STDOUT_FILENO) == -1) {
			perror("dup2 error");
			return 11;
		} else if (close(fd[0])) {
			perror("close pipe error");
			return 12;
        }
        else if (close(fd[1])) {
			perror("close pipe error");
			return 13;
        }
		else {  
			execl("./child", "./child", (char *) NULL);
		}
	} 
    else{
		if (close(file)) {
			perror("close file error");
			return 13;
		}	
		float res;
		if  (read(fd[0], &res, sizeof(res)) == -1){
            perror("read file error");
            return 15;
        }
        close(fd[0]);
        printf("[%d] Result from child: %f\n", getpid(), res);
    }
    if (close(fd[1])){
        	perror("close pipe error");
        	return 15;
    }
	return 0;
}