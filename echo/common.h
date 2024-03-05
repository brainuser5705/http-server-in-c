#ifndef SERVER_H
#define SERVER_H

#include <sys/un.h> // struct sockaddr_un

#define PATHNAME    "./socket"

struct sockaddr_un serverAddr =
    { .sun_family=AF_UNIX, .sun_path=PATHNAME };

void terminate_socket();

#define EXIT_IF_ERR(func, err_msg)  \
{                                   \
    int status = func;              \
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
