#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

float summ (char *array)
{    
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



int data_to_string(char** str, int* str_buf, int* str_size){
	char c;
    int res;
	
    char* new_str =  malloc(*str_buf * sizeof(char));

	while (1) {
        if((res = read(STDIN_FILENO, &c, sizeof(char))) > 0 && c != '\n') {
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


int main(int argc,char* argv[]){
    int str_size = 0;
    int alloc_memory_size = sizeof(float) * 1024;
    int str_buff = 1;
    char* new_str = NULL;
    assert(data_to_string(&new_str, &str_buff, &str_size) != -1);
    float res = summ(new_str);
    int file_message = open("msg.bin",O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    assert(file_message != -1);
    float* f_ptr = mmap(NULL,alloc_memory_size, 
                    PROT_READ |PROT_WRITE , MAP_SHARED,
                    file_message, 0);
    assert(ftruncate(file_message,alloc_memory_size) != -1);
    // как при помощи mmap передать на стандартный вход одной команды стандартный выхд другой
    // и наоборот как со стандартного выхода одной программы предать данные другой
    return 0;
}
