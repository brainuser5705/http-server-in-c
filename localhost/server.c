#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // htonl

#include "common.h"

#define BACKLOG     10

int serverFd;

int main(void){

    // tcp socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    // struct in_addr address = { .s_addr=ADDRESS };

    // create the socket address
    serverAddr_in4 = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    serverAddr_in4->sin_family = AF_INET;
    serverAddr_in4->sin_port = htons(PORT);
    serverAddr_in4->sin_addr = (struct in_addr) {.s_addr=htonl(ADDRESS)};

    EXIT_IF_ERR(
        bind(serverFd,(struct sockaddr *)serverAddr_in4,sizeof(*serverAddr_in4)),
        "Could not bind to localhost port ");

    EXIT_IF_ERR(listen(serverFd, BACKLOG), "Unable to listen");
    puts("Listening for incoming connections...");

    struct sockaddr_in *client =  (struct sockaddr_in *)malloc(sizeof(struct
        sockaddr_in));
    socklen_t clientAddrLen = 0;

    EXIT_IF_ERR(
        accept(serverFd, (struct sockaddr *) client, &clientAddrLen),
        "Cannot accept requests");
    puts("Accepted connection!\n");

    int clientFd = status;
    printf("Client: %d\n", clientFd);

/*
    struct sockaddr_in *test = (struct sockaddr_in *)malloc(
        sizeof(struct sockaddr_in));
    socklen_t  testSize = sizeof(struct sockaddr_in);
    EXIT_IF_ERR(getsockname(serverFd, (struct sockaddr *)test, &testSize),
        "Could not get socketname");
    
    printf("Size of test: %d\n", testSize);
    printf("The address of the socket is: %d\n", 
       (int *)((struct sockaddr *)test)->sa_data);
*/

    WAIT_FOR_ENTER("Press enter to close socket");

    terminate_socket();

    return 0;

}

void terminate_socket(){

    free(serverAddr_in4);
    
    // cannot use EXIT_IF_ERR otherwise recursive effect
    if (close(serverFd)){
        perror("Could not close socket");
        exit(-1);
    }

}
