#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc,char *argv[]){
    if (argc < 2){
        perror("Need filename argument in comand line\n");
        return 1;
    }
    const char* filepath = argv[1];
    int fd = open(filepath, O_RDONLY);
    if (fd == -1){
        perror("file doesnt open filedescriptor error\n");
        return 2;
    }
    struct stat statbuf;
    if (fstat(fd,&statbuf) == -1){
        perror("Cant take file satus data, fstat error\n");
        return 3;
    }
    
    void *ptr = mmap(NULL,statbuf.st_size,
                     PROT_READ | PROT_WRITE, MAP_PRIVATE,
                     fd, 0);
    if (ptr == MAP_FAILED){
        perror("Maping was faild, mmap fail\n");
        return 4;
    }
    if (write(1, ptr, statbuf.st_size) != statbuf.st_size){
        perror("Writing error file\n");
        return 5;
    }
    if(munmap(ptr, statbuf.st_size) == -1){
        perror("munmap failed\n");
        return 6;
    }
    
    return 0;
}