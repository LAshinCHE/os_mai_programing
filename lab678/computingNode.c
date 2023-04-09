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
#include <errno.h>
#include "RC.h"


extern int errno;

enum ComandType { Exec = 1,Ping = 2, Remove = 3,OK = 4,Create= 5 };

typedef struct
{
    enum ComandType command;
    int nodeId;
    int processId;
    char str[322];
    char subStr[322];
} messageData;

#define CONTROLLER_NODE_ID 4040

int main(int argc, char* argv[]){

    // Подписка на родителя
    int childNodeId = atoi(argv[1]);
    int parentNodeId = atoi(argv[2]);

    void *context = zmq_ctx_new();
    void *subscriber = zmq_socket(context, ZMQ_SUB);

    char adressParentNode[322];
    snprintf(adressParentNode, sizeof(adressParentNode), "tcp://localhost:%d", 4040 + parentNodeId);

    int conn;
    if(parentNodeId == -1)
        conn = zmq_connect(subscriber, "tcp://localhost:4040"); 
    else
        conn = zmq_connect(subscriber, adressParentNode);

    if (conn == -1)
        perror("conection faild");   

    conn = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, 0, 0);
    
    // Подключение дочернего узла
    void* context2 = zmq_ctx_new();
	void* publisher = zmq_socket(context2, ZMQ_PUB);

    char addresChildNode[256];
    snprintf(addresChildNode,sizeof(addresChildNode),"tcp://*:%d",4040 + childNodeId);
    zmq_bind (publisher, addresChildNode);
    /*
    zmq_msg_t reply;
	zmq_msg_init(&reply);
    zmq_msg_recv(&reply, subscriber, 0); 
    int curentPID;
    messageData* data2 = malloc(zmq_msg_size(&reply));
    memcpy(data2, zmq_msg_data(&reply), zmq_msg_size(&reply));
    if (data2->command == Create){

            if(data2->nodeId != childNodeId){
                zmq_msg_send(&reply, publisher, 0);
                zmq_msg_close(&reply);

                free(data2);
            }else{
                curentPID  = data2->processId;
                printf("PID:%d\n",curentPID);
            }
    }
    */

    for(;;){
        zmq_msg_t reply;
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, subscriber, 0); 

        messageData* data = malloc(zmq_msg_size(&reply));
        memcpy(data, zmq_msg_data(&reply), zmq_msg_size(&reply));
        if (data->command == Exec){

            if(data->nodeId != childNodeId){
                zmq_msg_send(&reply, publisher, 0);
                zmq_msg_close(&reply);

                free(data);
                continue;
            }else{
                sleep(10);

                long pozition  = RC(data->str, data->subStr);
	            printf("Ok:%d:%ld\n", childNodeId, pozition);

                zmq_msg_close(&reply);
            }
        }
        else if (data->command == Ping){
            if(data->nodeId == childNodeId){
                printf("OK: %d\n",childNodeId);
            }
            zmq_msg_send(&reply, publisher, 0);
            zmq_msg_close(&reply);

        }
        else if (data->command == Remove){

            if(data->nodeId == childNodeId || data->nodeId == -1){

                printf("Ok:%d\n", childNodeId);

                zmq_msg_close(&reply);
                free(data);
                
                messageData* data2 = malloc(sizeof(messageData));
                data2->command = Remove;
                data2->nodeId = -1;

                zmq_msg_t message;
                zmq_msg_init_size(&message, sizeof(messageData));

                memcpy(zmq_msg_data(&message), data2, zmq_msg_size(&message));

                zmq_msg_send(&message, publisher, 0);
            
		        zmq_msg_close(&message);
                
                free(data2);
                //char  killstr[64];
                //snprintf(killstr,sizeof(killstr),"sudo kill %d",curentPID);
                //printf("%s\n",killstr);
                //execl(killstr,killstr,NULL,(char*)NULL);
                break;
            } else {
                sleep(10);
                zmq_msg_send(&reply, publisher, 0);
                zmq_msg_close(&reply);
            }

        }

        free(data);
    }
    zmq_close(subscriber);
    zmq_close(publisher);
	zmq_ctx_destroy(context);
    zmq_ctx_destroy(context2);

    return 0;
}