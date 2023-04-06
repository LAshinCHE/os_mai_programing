#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include<assert.h>

#include "nodeArray.h"



enum ComandType {Create = 0,Exec = 1, Remove = 2};

typedef struct
{
    enum ComandType ct; 
    int nodeId;
    char str[322];
    char subStr[322];
} message;

#define CONTROLLER_NODE_ID 3222

int main(){

 return 0;
}