#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> //AP_INET protocol family
#include <unistd.h>
#include <arpa/inet.h> // htons, htonl

#include "common.h"

#define BACKLOG     10

int serverFd;

/* Socket address of server program */
// cannot be placed in common.h because initializers cannot be used for
// constants
// could be used a local variable, which doesn't require malloc
struct sockaddr_in *serverAddr_in4; 


int main(void){

    // tcp socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    // create the socket address
    serverAddr_in4 = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    serverAddr_in4->sin_family = AF_INET;
    serverAddr_in4->sin_port = htons(PORT);
    serverAddr_in4->sin_addr = (struct in_addr) {.s_addr=htonl(ADDRESS)};

    // bind socket to localhost port 80
    EXIT_IF_ERR(
        bind(serverFd,(struct sockaddr *)serverAddr_in4,sizeof(*serverAddr_in4)),
        "Could not bind to localhost port ");

    // set to listening mode
    EXIT_IF_ERR(listen(serverFd, BACKLOG), "Unable to listen");
    puts("Listening for incoming connections...");

    struct sockaddr_in *client =  (struct sockaddr_in *)malloc(sizeof(struct
        sockaddr_in));
    socklen_t clientAddrLen = 0;

    // accept connection requests
    EXIT_IF_ERR(
        accept(serverFd, (struct sockaddr *) client, &clientAddrLen),
        "Cannot accept requests");
    puts("Accepted connection!\n");

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
