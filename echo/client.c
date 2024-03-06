#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "common.h"

int clientFd;

int main(void){

    clientFd = socket(AF_UNIX, SOCK_STREAM, 0);

    EXIT_IF_ERR(
        connect(clientFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)),
        "Unable to connect to given address "PATHNAME);
    puts("Connected to "PATHNAME);

    char* in = NULL;
    size_t in_len;

    while(1){
        printf("> ");

        int numChars = getline(&in, &in_len, stdin);
        if (numChars == -1){
            free(in);
            puts("Error occured for input, try again...");
        } 

        // Proceed to send data over socket
        EXIT_IF_ERR(
            write(clientFd, in, sizeof(in)), "Unable to send data");

        // Check if newline after sending so server will know to terminate
        if (strcmp(in, "\n") == 0){
            break;
        }

    }

    terminate_socket();

        
    return 0;
}

void terminate_socket(){

    if (close(clientFd)){
        perror("Unable to close socket");
        exit(-1);
    }

}

