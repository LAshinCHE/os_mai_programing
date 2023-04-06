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

// requester - заказчик
// responder - ответчик

enum ComandType {Create = 0,Exec = 1, Remove = 2};

typedef struct
{
    enum ComandType ct; 
    int nodeId;
    char str[322];
    char subStr[322];
} message;

#define CONTROLLER_NODE_ID 3222

/*
В каждой ноде будем хранить число нод от которых данная нода зависит

Создание ноды
creat [id] [parentid]
После создание ноды отправляем родителю parentid сына данной ноды.
через exec просто создаем ноду которой передаем id и parentId(проверив на то что нода существует)



*/


int main(){


    void *context = zmq_ctx_new ();
    void *controller = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (controller, "tcp://*:3222");
    assert (rc == 0);


    char input[20];
    int id;
    int parentID;

    nodeArray nodes;
    if(nodeArrayInint(&nodes) == NodeInitError){
        return 2;
    }

    while(scanf("%s",input) != 0){

        if(strcmp (input, "create") == 0){

            if(nodeAllreadyExist(id, &nodes) == NodeExist){
                printf("This node allready in array\n");
            }
            else{
                scanf("%d  %d",id,parentID);
                // Creating new node in child procces
                pid_t pid = fork();
                if (pid == -1){
                    perror("Creating new node Error, cant create child procces\n");
                    continue;
                }
                if(pid > 0){
                    
                    printf("OK:%d\n",id);
                    if(addNode(&nodes,id) == AddNodeError){
                        perror("Node doesnt add in common nodes array\n");
                        return 2;
                    }
                }
                if(pid == 0){
                    
                    char parentNodeID[322];
                    snprintf(parentNodeID,sizeof(parentNodeID),"%d",parentID);

                    char curNodeID[322];
                    snprintf(curNodeID,sizeof(curNodeID),"%d",id);

                    if(execl("./compNode", "./compNode",curNodeID,parentNodeID, (char *) NULL) == -1){
                        printf("Creating new node Error, cant executing new node\n");
                        continue;
                    }
                }
            }
        }
        else if(strcmp (input, "exec") == 0){
            printf("Do some exec\n");
        }
        else if(strcmp (input, "remove") == 0){
            printf("Do some remove!\n");
        }
        else if(strcmp (input, "quit") == 0){
            printf("Bye Bye\n");
        }
             

        else{
            perror("Wrong command!\n");
            return 1;
        }
    }
    return 0;
}