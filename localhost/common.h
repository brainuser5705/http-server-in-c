#ifndef LOCALHOST_COMMON_H
#define LOCALHOST_COMMON_H

#include <netinet/in.h>

#include <errno.h>

#define PORT    80  //< default port for HTTP
#define ADDRESS INADDR_LOOPBACK //< localhost address IPv4

/*
struct sockaddr_in6 serverAddr_in6 = {
    .sin6_family=AF_INET6,
    .sin6_port=80,
    .sin6_flowinfo=0,   // no flow
    .sin6_addr=IN6ADDR_LOOPBACK_INIT, // localhost
    .sin6_scope_id=,
};
*/

/**
    Terminates the current connection by closing socket.
    Procedure for termination is specific to the program.
*/
void terminate_socket();

/**
    Return value of function used with macro EXIT_IF_ERR 
    It is exposed because some function return a value that
    does not indicated pass/fail e.g. a file descriptor
    from an accepted socket connection request.

    The program would assign this variable to a program
    specific variable for usage.
*/
int status;

#define EXIT_IF_ERR(func, err_msg)      \
{                                       \
    status = func;                      \
    if (status == -1){                  \
        printf("errno: %d\n", errno);   \
        perror(err_msg);                \
        terminate_socket();             \
        exit(-1);                       \
    }                                   \
}                                   

#define WAIT_FOR_ENTER(msg)     \
{                               \
    puts(msg);                  \
    while (getchar() != '\n');  \
}                               


#endif
