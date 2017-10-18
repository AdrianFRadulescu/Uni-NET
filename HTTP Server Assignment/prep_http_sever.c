#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>
#include <tclDecls.h>
#include "stdlib.h"

char web_page[] =
"HTTP/1.1 200 OK\r\n"
"Content-Type: text/html charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title></title></html>"
"";
/*
int make_socket (uint16_t port) {
    int socket_desc;
    struct sockaddr_in name;

    // Create the socket.
    socket_desc = socket (PF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror ("socket");
        exit(EXIT_FAILURE);
    }

    // Give the socket a name.
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);

    if (bind (socket_desc, (struct sockaddr *) &name, sizeof (name)) < 0) {
        perror ("bind");
        exit (EXIT_FAILURE);
    }
    return socket_desc;
}
*/

int main(int argc , char *argv[]) {

    char* end;
    //int socket_desc = make_socket((uint16_t) strtol(argv[1], &end, 10));
    /*
    if (listen(socket_desc, 0) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }*/

    // puts("Waiting for incoming connections");

    /*int c = sizeof(struct sockaddr_int);
    struct sockaddr_in client;
    int new_socket = accept(socket_desc, (struct sockaddr*) &client, (socklen_t*) &c);

    if (new_socket < 0){
        puts("connection failed");
    }*/

    //puts("Connection accepted");

    //shutdown(socket_desc, 2);
    return 0;
}