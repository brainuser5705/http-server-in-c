#ifndef SERVER_H
#define SERVER_H

#include <sys/un.h> // struct sockaddr_un

#define PATHNAME    "./socket"

/* Socket address of server program */
struct sockaddr_un serverAddr =
    { .sun_family=AF_UNIX, .sun_path=PATHNAME };

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

#define EXIT_IF_ERR(func, err_msg)  \
{                                   \
    status = func;              \
    if (status == -1){    \
        perror(err_msg);            \
        terminate_socket();         \
        exit(-1);                   \
    }                               \
}                                   

#define WAIT_FOR_ENTER(msg)     \
{                               \
    puts(msg);                  \
    while (getchar() != '\n');  \
}                               


#endif
