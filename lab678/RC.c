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
#include <math.h>
#include "nodeArray.h"
#define CAPACITY 5000000

static long getHash(char* str,long left, long right) {
   	long hash = 0;
	int p = 13;
    for (int i=left; i <= right; i++)
        hash += str[i]*pow(p,i-left);
    return hash % CAPACITY;
}



long RC(char* str,char* sample){
	long sampleLen = strlen(sample);
	long sampleHash = getHash(sample,0,sampleLen - 1);
	long strLen = strlen(str);

	for (long pozition = 0 ; pozition < strLen; pozition++)
	{
		long substringHash = getHash(str,pozition,sampleLen + pozition - 1);
		if (substringHash == sampleHash){
			printf("Searching '%s' in '%s' result = %ld  \n",str,sample, pozition);
			return pozition;
		}
		
	}
	printf("Searching '%s' in '%s' result = %d  \n",str,sample, -1);
	return -1;
}