#include "RC.h"
#include <math.h>
#include <string.h>

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
			return pozition;
		}
		
	}
	
	return -1;
}