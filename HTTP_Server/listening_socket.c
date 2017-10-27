//
// Created by Radulescu Adrian on 18/10/2017.
//


#include "listening_socket.h"


int create_listening_socket (const uint16_t port, const int max_conn_number) {
    int socket_desc;
    struct sockaddr_in6 name;

    //zero the address
    memset(&name, 0, sizeof(name));

    // Create the socket.
    socket_desc = socket (PF_INET6, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror ("socket");
        exit(EXIT_FAILURE);
    }

    // Give the socket a name.
    memset (&name, '\0', sizeof (name));
    name.sin6_family = AF_INET6;
    name.sin6_addr = in6addr_any;
    name.sin6_port = htons(port);

    const int one = 1;

    if (setsockopt (socket_desc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one)) != 0) {
        perror ("setsocket");
        /* but carry on, as it is only a 'nice to have' */
    }

    if (bind (socket_desc, (struct sockaddr *) &name, sizeof (name)) < 0) {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    if (listen(socket_desc, max_conn_number)){
        perror("Error on listen");
        exit(EXIT_FAILURE);
    }

    return socket_desc;
}


