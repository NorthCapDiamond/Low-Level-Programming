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
    Answer* message;

    for (;;){
        bytes_received = recv(sockfd, &buffer, 1024, 0);
        if(bytes_received > 0){
            printf("Recevied Bytes : %llu\n", bytes_received);
            flag = 1;
        }


        message = answer__unpack(NULL, bytes_received, buffer);

        if (message == NULL) {
            fprintf(stderr, "Error unpacking the message\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if(message->id){
            printf("ID : %llu  *(may mean amount of attributes if substr or string eq is used)\n", message->id);
        }

        if(message->uint_attrs){
            printf("Uint Attributes : %s\n", message->uint_attrs);
        }

        if(message->double_attrs){
            printf("Double Attributes : %s\n", message->double_attrs);
        }

        if(message->string_attrs){
            printf("String Attributes : %s\n", message->string_attrs);
        }

        if(message->msg){
             printf("Message from programm for you : %s *(may not be used)\n", message->msg);
        }

        answer__free_unpacked(message, NULL);

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