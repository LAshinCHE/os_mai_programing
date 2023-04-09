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
#include "RC.h"
// requester - заказчик
// responder - ответчик

enum ComandType { Exec = 1,Ping = 2, Remove = 3,OK = 4, Create= 5};

typedef struct
{
    enum ComandType command; 
    int nodeId;
    int processId;
    char str[322];
    char subStr[322];
} messageData;

#define CONTROLLER_NODE_ID 4040

/*
В каждой ноде будем хранить число нод от которых данная нода зависит

Создание ноды
creat [id] [parentid]
После создание ноды отправляем родителю parentid сына данной ноды.
через exec просто создаем ноду которой передаем id и parentId(проверив на то что нода существует)



*/


int main(){

	void* context = zmq_ctx_new();
	void* publisher = zmq_socket(context, ZMQ_PUB);
    int rc = zmq_bind (publisher, "tcp://*:4040");
    assert (rc == 0);


    char input[20];
    int id;
    int parentID;

    nodeArray nodes;
    if(nodeArrayInint(&nodes) == NodeInitError){
        return 2;
    }

    int controlNode = 0;
    addNode(&nodes,controlNode);


    int relation[2][256];
    for(int i = 0; i < 256; i++){
        relation[0][i] = 0;
        relation[1][i] = 0;
    }
    int iter_relation = 0;

    while(scanf("%s",input) != EOF){

        if(!strcmp (input, "create")){

            scanf("%d",&id);
            if(nodeAllreadyExist(id, &nodes) == NodeExist){
                printf("This node allready in array: %d\n", id);
                continue;
            }
            scanf("%d",&parentID);
            if(nodeAllreadyExist(parentID, &nodes) == NodeDoesNotExist){
                printf("Error: Parent not found\n");
                continue;
            }

            relation[0][iter_relation] = parentID;
            relation[1][iter_relation] = id;
            iter_relation++;

            //messageData* data = malloc(sizeof(messageData));

            // Creating new node in child procces
            pid_t pid = fork();
            if (pid == -1){
                perror("Creating new node Error, cant create child procces\n");
                continue;
            }
            if(pid > 0){
                /*sleep(10);
                data->processId = pid;
                data->nodeId = id;
                data->command=Create; 
                zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(messageData));

                memcpy(zmq_msg_data(&message), data, zmq_msg_size(&message));

                zmq_msg_send(&message, publisher, 0);
            
		        zmq_msg_close(&message);
                */
                printf("OK:%d\n",pid);
                if(addNode(&nodes,id) == AddNodeError){
                    perror("Node doesnt add in common nodes array\n");
                    continue;
                }
            }
            if(pid == 0){
                char parentNodeID[322];
                snprintf(parentNodeID,sizeof(parentNodeID),"%d",parentID);

                char curNodeID[322];
                snprintf(curNodeID,sizeof(curNodeID),"%d",id);

                if(execl("./compNode", "./compNode", curNodeID,parentNodeID, (char *) NULL) == -1){
                    printf("Creating new node Error, cant executing new node\n");
                    continue;
                }
            }
        }
        else if(!strcmp (input, "exec")){
            int node_number;
            scanf("%d", &node_number);
            if(nodeAllreadyExist(node_number, &nodes) == NodeDoesNotExist){
                printf("This node allready in array: %d\n", node_number);
                continue;
            }
            char line[322];
            char sample[322];
            messageData* data = malloc(sizeof(messageData));

            scanf("%s", line);
            scanf("%s", sample);

            strcpy(data->str, line);
            strcpy(data->subStr, sample);
            
            data->command = Exec;
            data->nodeId = node_number;

            zmq_msg_t message;
            zmq_msg_init_size(&message, sizeof(messageData));

            memcpy(zmq_msg_data(&message), data, zmq_msg_size(&message));

            zmq_msg_send(&message, publisher, 0);
            
		    zmq_msg_close(&message);

            free(data);
        }
        else if(strcmp (input, "remove") == 0){
            int node_number;
            scanf("%d", &node_number);

            if(nodeAllreadyExist(id, &nodes) == NodeDoesNotExist){
                printf("ID not found\n");
                continue;
            }

            messageData* data = malloc(sizeof(messageData));
            data->command = 3;
            data->nodeId = node_number;

            zmq_msg_t message;
            zmq_msg_init_size(&message, sizeof(messageData));

            memcpy(zmq_msg_data(&message), data, zmq_msg_size(&message));

            zmq_msg_send(&message, publisher, 0);
            
		    zmq_msg_close(&message);

            free(data);

            for(int i = 0; i < 256; i++){
                if (nodes.data[i] == node_number){
                    nodes.data[i] = 0;
                    break;
                }
            }

            for(int i = 0; i < 256; i++){
                if (relation[0][i] == node_number){
                    for(int j = 0; j < 256; j++){
                        if (nodes.data[j] == relation[1][i]){
                            nodes.data[j] = 0;
                            break;
                        }
                    }
                }
                if (relation[1][i] == node_number){
                    relation[0][i] = 0;
                    relation[1][i] = 0;
                }
            }

            if(node_number == -1){
                printf("Programm stopped\n");
                break;
            }
        
        }
        else if(strcmp (input, "ping") == 0){
            
            int pingingId;
            scanf("%d",&pingingId);
            if (nodeAllreadyExist(pingingId,&nodes) == NodeDoesNotExist)
                printf("Error: not found\n");
            
            messageData* data = malloc(sizeof(messageData));
            data->command = Ping;
            data->nodeId = pingingId;

            zmq_msg_t message;
            zmq_msg_init_size(&message, sizeof(messageData));

            memcpy(zmq_msg_data(&message), data, zmq_msg_size(&message));

            zmq_msg_send(&message, publisher, 0);
            
		    zmq_msg_close(&message);
            free(data);
        }
        else if(strcmp (input, "quit") == 0){
            printf("Bye Bye\n");
        }    
        else{
            printf("Wrong command!: '%s'\n",input);
            continue;
        }
    }
    return 0;
}