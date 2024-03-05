#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> // socket operations
#include <sys/un.h>     // UNIX protocol family
#include <unistd.h>
#include <errno.h>
#include <poll.h>       // use for polling changes to client socket
#include <string.h>

#include "common.h"

#define BACKLOG             10      //< for listen()
#define POLL_NUM_CLIENTS    1       //< for poll()
#define POLL_TIMEOUT        10000
#define MAX_MESSAGE_LEN     100     //< for message buffer

int serverFd;

int main(void){

    // Create socket
    serverFd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    // Bind to address
    EXIT_IF_ERR(
        bind(serverFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)),
        "Unable to bind to "PATHNAME);    

    // Set socket to listen (turns socket to passive socket)
    EXIT_IF_ERR(listen(serverFd, BACKLOG), "Unable to listen");
    puts("Listening for incoming connections...");
    
    // Blocking socket accepting connection request
    struct sockaddr_un *client = (struct sockaddr_un *)malloc(sizeof(struct 
        sockaddr_un));
    socklen_t clientAddrLen = 0;

    int clientFd = accept(serverFd, (struct sockaddr *)client, &clientAddrLen);
    if (clientFd == -1){
        perror("Unable to accept connection requests");
        terminate_socket();
        exit(-1);
    }

    puts("Accepted connection, listening for data...");

    // Continuously polling for requests from client
    struct pollfd clientPoll = 
        { .fd=clientFd, .events=POLLIN };
    
    char msg[MAX_MESSAGE_LEN];
    while (1){

        int pollStatus = poll(&clientPoll, POLL_NUM_CLIENTS, POLL_TIMEOUT);

        if (pollStatus == -1){
            perror("Unable to poll for data writes");
            terminate_socket();
            exit(-1);
        }else if (pollStatus == 0){
            continue;
        }

        read(clientFd, msg, sizeof(msg));

        if (strcmp(msg, "\n") == 0){
            break;
        }
        printf("client says: %s", msg);

    }

    terminate_socket();

    return 0;

}


void terminate_socket(){

    if(unlink(PATHNAME)){
        perror("Unable to unllink "PATHNAME);
        exit(-1);
    }

    if (close(serverFd)){
        perror("Unable to close socket");
        exit(-1);
    }

}
