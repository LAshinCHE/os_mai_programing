#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <errno.h>

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
    errno = 0;
	
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
        return errno;
    free(*str);
    *str = new_str;
    return 0;
}


int main(){
    int str_size = 0;
    int str_buff = 1;
    char* new_str = NULL;
    data_to_string(&new_str, &str_buff, &str_size);
    float res = summ(new_str);
    printf ("%lf \n", res);
	if ((write(STDOUT_FILENO, &res, sizeof(res)) == -1))
		perror("write error");
    
    return 0;
}
