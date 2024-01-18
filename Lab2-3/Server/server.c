#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.h"

#define MAX 80 
#define PORT 3110
#define SA struct sockaddr


extern int flag_parse;
extern Node* my_answer;


void send_message(Node msg, int sockfd){
    uint64_t message_size = node__get_packed_size(&msg);
    printf("Message size is %llu\n", message_size);
    uint8_t* buffer = malloc(message_size);
    node__pack(&msg, buffer);
    send(sockfd, buffer, message_size, 0);
    free(buffer);
}

void server_jobs(int connfd, struct sockaddr_in cli){
    // Receive data from the client
    int flag = 1;

    Node first_message = NODE__INIT;
    first_message.msg = "Ready to start";
    printf("%s\n", first_message.msg);
    printf("First Message sent\n\n\n");

    send_message(first_message, connfd);

    for(;;){
        uint8_t buffer[1024];
        uint64_t bytes_received = recv(connfd, &buffer, 1024, 0);
        if (!bytes_received) {
          printf("Disconnected!\n");
          return; 
        }



        printf("Bytes Received : %llu\n", bytes_received);
        Ast* msg = ast__unpack(NULL, bytes_received, buffer);

        if (msg == NULL) {
            fprintf(stderr, "Error unpacking the message\n");
            close(connfd);
            exit(EXIT_FAILURE);
        }


        flag_parse = 0;
        printf("Received Message:\n");
        my_answer = run_request(msg);

        printf("Try to printf\n");


        send_message(*my_answer, connfd);
        printf("Sent from server\n");


        

        // Clean up
        ast__free_unpacked(msg, NULL);
    }

   
    // After chatting close the socket 
    close(connfd);
    exit(0);
}
   
int main(){
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
   
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
   
    // Accept the data packet from client and verification 

    listen_loop:;
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed...\n"); 
        exit(0); 
    }
    else{
        printf("server accept the client... Trying to fork\n");
        if(!fork()){
            printf("Got Client connection: %d %s\n", connfd, inet_ntoa(cli.sin_addr));
            printf("moving down to commands\n");
            server_jobs(connfd, cli);
        }
        else {
            goto listen_loop;
        }
    }
}