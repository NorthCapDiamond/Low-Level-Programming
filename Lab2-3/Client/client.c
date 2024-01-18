#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include "printer.h"
#include "parser.tab.h"
#include "converter.h"
#include "input.pb-c.h"

#define MAX 80
#define PORT 3110
#define SA struct sockaddr

extern int yyparse();
extern ast_node* ast;

void send_message(Ast msg, int sockfd){
    uint64_t message_size = ast__get_packed_size(&msg);
    printf("Message size is %llu\n", message_size);
    uint8_t* buffer = malloc(message_size);
    ast__pack(&msg, buffer);
    send(sockfd, buffer, message_size, 0);
    free(buffer);
}
 
int main(){
    int sockfd;
    struct sockaddr_in servaddr;
 
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        exit(0);
    }



    int flag = 0;

    //send_message(*msg, sockfd);
    uint8_t buffer[1024];
    uint64_t bytes_received;
    Node* message;

    for (;;){
        bytes_received = recv(sockfd, &buffer, 1024, 0);
        if(bytes_received > 0){
            printf("Recevied Bytes : %llu\n\n\n", bytes_received);
            flag = 1;
        }


        message = node__unpack(NULL, bytes_received, buffer);

        if (message == NULL) {
            fprintf(stderr, "Error unpacking the message\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if(message->id){
            printf("\nID of node : %llu\n", message->id);
        }

        if(message->d){
            printf("D of node: %llu\n", message->d);

            UintArray* array= message->nodes;

            for(uint64_t i = 0; i < message->d; i++){
                printf("EL%llu : %llu\n", i+1, array->value);
                array = array->next;
            }
        }

        if(message->n_ints){
            printf("Amount of node ints : %llu\n", message->n_ints);

            UintArray* array = message->ints_array_ptr;

            for(uint64_t i = 0; i < message->n_ints; i++){
                printf("EL%llu : %llu\n", i+1, array->value);
                array = array->next;
            }
        }

        if(message->n_doubles){
            printf("Amount of node doubles : %llu\n", message->n_doubles);

            DoubleArray* array = message->doubles_array_ptr;

            for(uint64_t i = 0; i < message->n_doubles; i++){
                printf("EL%llu : %f\n", i+1, array->value);
                array = array->next;
            }
        }

        if(message->n_strings){
            printf("amount of node strings  : %llu\n", message->n_strings);

            StringArray* array = message->strings_array_ptr;

            for(uint64_t i = 0; i < message->n_strings; i++){
                printf("EL%llu : %s\n", i+1, array->value);
                array = array->next;
            }
        }

        if(message->msg){
             printf("Message from programm for you : %s\n", message->msg);
        }

        if(message->next_node!=NULL){
            printf("\nIncluded Nodes:\n");
            while(message->next_node!=NULL){
                    message = message->next_node;
                    if (message == NULL) {
                    fprintf(stderr, "Error unpacking the message\n");
                    close(sockfd);
                    exit(EXIT_FAILURE);
                }
                if(message->id){
                    printf("ID of node : %llu\n", message->id);
                }

                if(message->d){
                    printf("D of node: %llu\n", message->d);

                    UintArray* array= message->nodes;

                    for(uint64_t i = 0; i < message->d; i++){
                        printf("EL%llu : %llu\n", i+1, array->value);
                        array = array->next;
                    }
                }

                if(message->n_ints){
                    printf("Amount of node ints : %llu\n", message->n_ints);

                    UintArray* array = message->ints_array_ptr;

                    for(uint64_t i = 0; i < message->n_ints; i++){
                        printf("EL%llu : %llu\n", i+1, array->value);
                        array = array->next;
                    }
                }

                if(message->n_doubles){
                    printf("Amount of node doubles : %llu\n", message->n_doubles);

                    DoubleArray* array = message->doubles_array_ptr;

                    for(uint64_t i = 0; i < message->n_doubles; i++){
                        printf("EL%llu : %f\n", i+1, array->value);
                        array = array->next;
                    }
                }

                if(message->n_strings){
                    printf("amount of node strings  : %llu\n", message->n_strings);

                    StringArray* array = message->strings_array_ptr;

                    for(uint64_t i = 0; i < message->n_strings; i++){
                        printf("EL%llu : %s\n", i+1, array->value);
                        array = array->next;
                    }
                }

                if(message->msg){
                     printf("Message from programm for you : %s\n", message->msg);
                }
            }
        }

        node__free_unpacked(message, NULL);

        if(flag){
            yyparse();
            Ast* msg = convert(ast);
            flag = 0;
            send_message(*msg, sockfd);
            printf("Sent from client\n");
        }
    }

    return 0;
}