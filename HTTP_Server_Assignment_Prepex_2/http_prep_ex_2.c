#include <stdio.h>
//#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread/pthread.h>
#include <arpa/inet.h>

#define BUFFER_LENGTH 256

int is_executed = 0;
pthread_mutex_t lock;



int create_socket (uint16_t port) {
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

void *thread_handling(void* args){

    fprintf(stderr, "Thread is executing\n");
    int socket_desc = *(int*) args;
    char buffer[BUFFER_LENGTH];

    char* welcome_message = "Hi, I'm your connection handler! Don't buffer overflow me please!\n";

    write(socket_desc, welcome_message, strlen(welcome_message));

    read(socket_desc, buffer, BUFFER_LENGTH);

    fprintf(stderr,"Message from client is: %s",buffer);

    //Free the socket pointer
    free(args);
    pthread_exit(NULL);
}


void traffic_info(const int socket_desc){

    socklen_t len;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    len = sizeof addr;
    getpeername(socket_desc, (struct sockaddr*)&addr, &len);

    // deal with both IPv4 and IPv6:
    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    }

    // AF_INET6
    else {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    }

    fprintf(stderr, "Peer IP address: %s\nPort: %d", ipstr, port);

}

int main(int argc, char *argv[]) {

    int const i = 4;
    int j;
    j = i + 1;
    char* end;
    int socket_desc = create_socket((uint16_t) strtol(argv[1], &end, 10));
    struct sockaddr_in client_addr;
    socklen_t client_len;

    if (listen(socket_desc, 5) < 0){
        perror("Error on listen");
        exit(EXIT_FAILURE);
    }

    while(1){

        pthread_t server_thread;
        // attributes for the newly created thread
        pthread_attr_t new_thread_attr;

        //allocate memory for each instance to avoid race conditions
        int *new_socket_desc = malloc(sizeof(int));
        if (!new_socket_desc) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        // waiting for connections
        *new_socket_desc = accept(socket_desc, (struct sockaddr*) &client_addr,  &client_len);
        if (*new_socket_desc < 0){
            perror("ERROR on Accept");
            exit(EXIT_FAILURE);
        }

        // create separate thread for processing
        if (pthread_attr_init(&new_thread_attr)){
            perror("Creating initial thread attributes failed");
            exit(EXIT_FAILURE);
        }

        if (pthread_attr_setdetachstate(&new_thread_attr, PTHREAD_CREATE_DETACHED)){
            perror("Setting thread attributes failed");
            exit(EXIT_FAILURE);
        }


        if (pthread_create(&server_thread, &new_thread_attr, thread_handling, (void*) new_socket_desc)){
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }


        traffic_info(*new_socket_desc);

    }

    return 0;
}
