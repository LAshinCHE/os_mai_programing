#include <stdio.h>
#include <stdlib.h>
#include "nodeArray.h"

int nodeArrayInint(nodeArray* nodes){
    nodes->data = (int*)malloc(sizeof(int) * 1);
    if (nodes->data == NULL){
        perror("node doesnt init\n");
        return NodeInitError;
    }
    nodes->size = 0;
    nodes->capacity = 1;
    return NodesWorckClearly;
}


int getSize(nodeArray* node){
    return node->size;
}

int addNode(nodeArray* nodes,int node){

    if (nodes->data == NULL){
        perror("This node array doesnt exist!\n");
        return AddNodeError;
    }

    if (nodes->size + 1 <= nodes->capacity){
        nodes->data = (int*)realloc(nodes->data, sizeof(int) * nodes->capacity * 2);
        if (nodes->data == NULL){
            perror("Node dosesnt add error\n");
            return AddNodeError;
        }
        
        nodes->capacity = nodes->capacity * 2;
    }

    nodes->data[getSize(nodes)] = node;
    nodes->size = nodes->size + 1;
    
    return NodesWorckClearly;
}

int nodeAllreadyExist(int nodeId, nodeArray* nodes){
    int numberOfNodes = getSize(nodes);
    for (int i = 0; i < numberOfNodes; i++){
        if( nodes->data[i] == nodeId )
            return NodeExist;
    }
    return NodeDoesNotExist;
}