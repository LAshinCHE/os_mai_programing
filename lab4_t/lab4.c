/**
 * Sample code for sharing memory between processes
 * Two processes will iteratively increase a counter which values stored in a shared memory
 * 
 */


#include <stdio.h>
#include <unistd.h> // for fork()
#include <sys/mman.h> // for shared memory created
#include <sys/stat.h> // for mode constants
#include <fcntl.h> // for O_* constant
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<assert.h>

#define SHARED_OBJ_NAME "/message"

// shared data struct
typedef struct
{
    float summ;
} message;

bool write_message(float value){
    int shmFd = shm_open(SHARED_OBJ_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    assert(shmFd != -1);
    assert(ftruncate(shmFd, sizeof(message)) != -1);
    message *msg_ptr = (message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    assert(msg_ptr != MAP_FAILED);
    msg_ptr->summ = value;
    assert(munmap(msg_ptr, sizeof(message)) != -1);
    assert(close(shmFd) != -1);

    return true;
}

float read_message(){
    int shmFd = shm_open(SHARED_OBJ_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    assert(shmFd != -1);
    assert(ftruncate(shmFd, sizeof(message)) != -1);
    message *msg_ptr = (message*)mmap(NULL, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    assert(msg_ptr != MAP_FAILED);
    float curr_value = msg_ptr->summ;
    assert(munmap(msg_ptr, sizeof(message)) != -1);
    assert(close(shmFd) != -1);

    return curr_value;
}

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
    new_name = (char *)realloc(new_name,sizeof(char)*capacity + 1);
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


int data_to_string(char** str, int* str_buf, int* str_size,int fd){
	char c;
    int res;
	
    char* new_str =  malloc(*str_buf * sizeof(char));

	while (1) {
        if((res = read(fd, &c, sizeof(char))) > 0 && c != '\n' && c!='\0') {
            if (str_buf <= str_size){
                *str_buf *= 2;
                new_str = realloc(new_str,*str_buf * sizeof(char));
            }
            new_str[*str_size] = c;
            *str_size += 1;
		}
        else{
            break;
        }
    }
    new_str[*str_size] = '\0';
    if (res == -1)
        return -1;
    free(*str);
    *str = new_str;
    return 0;
}

float summ (char *array){    
    char* new_str;
    int str_buf;
    int str_size;
    float number = 0;
    float final_summ = 0;
    int iter = 0; 
    while (array[iter] != '\n' && array[iter] != '\0')
    {
        str_buf = 1; 
        str_size = 0;
        new_str = malloc(sizeof(char));
        while(array[iter] != '\0' && array[iter] != ' ' && array[iter] != '\n' && array[iter] != '\t'){
            if (str_buf <= str_size){
                str_buf *= 2;
                new_str = realloc(new_str, sizeof(char)*str_buf);
            }
            new_str[str_size] = array[iter]; 
            str_size += 1;
            iter+=1;
        }
        if (new_str  != NULL)
        {
            number = atof(new_str);
            final_summ += number;
        }
        free(new_str);
        iter+=1;
    }

    return final_summ;
}

int main(int argc, char **argv)
{
    char *filename = NULL;
    int file_len = 0;
    printf("Write file that u want to execute!\n");
    if(read_filename(&filename ,&file_len) == -1){
        perror("reading file error!\n");
        return 1;   
    }
    int file;
    if ((file  = open(filename, O_RDWR | O_CREAT, 0777))  == -1){
        perror("opening msg error!\n");
        return 2;
    }
    pid_t pid = fork();
    if (pid == -1){
        perror("forck errror!\n");
        return 3;
    }
    //--- CHILD PROCESS
    if (pid == 0){
        int str_size = 0;
        int str_buff = 1;
        char* new_str = NULL;
        data_to_string(&new_str, &str_buff, &str_size,file);
        float sum_f = summ(new_str);
        assert(write_message(sum_f) != false);
    }
    //--- PARENT PROCESS
    else{
		float read_data = read_message();
        if (close(file)) {
            perror("close file error");
            return 5;
		}	
        printf("[%d] Result from child: %f\n", getpid(), read_data);
    }
    //shm_unlink(SHARED_OBJ_NAME);

    return 0;
}
