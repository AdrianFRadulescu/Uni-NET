#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "listening_socket.h"
#include "connection_handling.h"


int main(const int argc, const char* argv[]) {

    // adjust path of the application
    pwd_adjust(argv[0], pwd);

    // get port

    char* end_;
    uint16_t port;

     if (argc == 2) {
        port = (uint16_t) strtol(argv[1], &end_, 10);
    } else {
        port = 3349;
    }

    if (1024 >= port || port > pow(2,31)) {
        perror("Incorrect port number");
        exit(EXIT_FAILURE);
    }


    int server_socket_desc = create_listening_socket(port, 10);

    while (create_client_thread(server_socket_desc) >= 0);

    shutdown(server_socket_desc, 2);
    //*/
    return 0;
}

