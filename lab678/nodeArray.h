#ifndef __RC_H__
#define __RC_H__



typedef struct 
{
    int* data;
    int capacity;
    int size;
} nodeArray;


enum NodeError {NodesWorckClearly  = 0, NodeInitError, AddNodeError, NodeExist, NodeDoesNotExist,NodeSuccessfullyDeleted, NodeDoesntDelet};



int nodeArrayInint(nodeArray* nodes);
int getSize(nodeArray* nodes);
int addNode(nodeArray* nodes,int node);
int nodeAllreadyExist(int nodeId, nodeArray* nodes);

#endif 