#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread/pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include <math.h>
#include <time.h>

#define BUFFER_LENGTH 256

static int run = 1, server_socket_desc;


char response_header[] =
    "HTTP/1.1 200 OK\n"
    "Server: bla bla\r\n"
    "Last-Modified: Tue, 01 May 2007 14:24:39 GMT\r\n"
    "ETag: 499fd34e-29ec-42f695ca96761;48fe7523cfcc1\r\n"
    "Accept-Ranges: bytes\r\n"
    "Content-Length: 10732\r\n"
    "Content-Type: text/html\n"
    "\n";



/*
char response_header[] =
    "HTTP/1.1 200 OK\n"
        "Content-type: text/html\n"
        "\n"
        "<html>\n"
        " <body>\n"
        "  <h1>Not Found123</h1>\n"
        "  <p>The requested URL was not found on this server.</p>\n"
        " </body>\n"
        "</html>\n";


char *reply =
    "HTTP/1.1 200 OK\n"
        "Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
        "Server: Apache/2.2.3\n"
        "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
        "ETag: \"56d-9989200-1132c580\"\n"
        "Content-Type: text/html\n"
        "Content-Length: 15\n"
        "Accept-Ranges: bytes\n"
        "Connection: close\n"
        "\n"
        "<html>\r\n"
        " <body>\r\n"
        "  <h1>Not Found</h1>\r\n"
        "  <p>The requested URL was not found on this server.</p>\r\n"
        " </body>\r\n"
        "</html>\r\n";
*/

int create_socket (const uint16_t port) {
    int socket_desc;
    struct sockaddr_in name;

    //zero the address
    memset(&name, 0, sizeof(name));

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

    if (listen(socket_desc, 5)){
        perror("Error on listen");
        exit(EXIT_FAILURE);
    }

    return socket_desc;
}

void* thread_handling(void* args){

    fprintf(stderr, "Thread is executing\n");
    int socket_desc = *(int*) args;
    char request[BUFFER_LENGTH];

    //char* welcome_message = "Hi, I'm your connection handler! Don't buffer overflow me please!\n";

    FILE* file_desc = fopen("test.html", "r");
    fseek(file_desc, 0, SEEK_END);
    long fsize = ftell(file_desc);
    fseek(file_desc, 0, SEEK_SET);

    char *file_content = malloc(sizeof(char) * (size_t) fsize + 1);
    fread(file_content, (size_t) fsize, 1, file_desc);


    char http_response[/*strlen(reply)*/strlen(response_header) + strlen(file_content)];
    printf("%zu %zu %zu\n",sizeof(http_response), sizeof(response_header), strlen(file_content) * sizeof(char));

    //strncpy(http_response, reply, strlen(reply) * sizeof(char));
    strncpy(http_response, response_header, sizeof(response_header));
    strcat(http_response,file_content);

    printf("sending:\n%s\n\n", http_response);

    send(socket_desc, http_response, strlen(http_response),0);

    read(socket_desc, request, BUFFER_LENGTH);

    //fprintf(stderr,"Request from client is:\n %s\n\n",request);

    free(file_content);


    if (strstr("GET /", request)){

        fprintf(stderr, "process req...\n\n");



    }

    //Free the socket pointer
    free(args);
    fprintf(stderr, "Shutting socket down to ");
    shutdown(socket_desc, 2);
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

void INT_handler(int sig_no) {
    if (sig_no == SIGINT){
        printf("Do you want to shut the server down(y/n)?\n");
        if (fgetc(stdin) == 'y') {
            shutdown(server_socket_desc, 2);
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {

    int const i = 4;
    int j;
    j = i + 1;
    char* end;

    uint16_t port;

    if (argc == 2) {
        port = (uint16_t) strtol(argv[1], &end, 10);
    } else {
        port = 2345;
    }

    if (1024 >= port || port > pow(2,31)) {
        perror("Incorrect port number");
        exit(EXIT_FAILURE);
    }

    server_socket_desc = create_socket(port);
    struct sockaddr_in client_addr;
    socklen_t client_len;

    signal(SIGINT, INT_handler);

    while(run){

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
        *new_socket_desc = accept(server_socket_desc, (struct sockaddr*) &client_addr,  &client_len);
        if (*new_socket_desc < 0){
            perror("ERROR on Accept");
            free(new_socket_desc);
            exit(EXIT_FAILURE);
        }

        // create separate thread for processing
        if (pthread_attr_init(&new_thread_attr)){
            perror("Creating initial thread attributes failed");
            free(new_socket_desc);
            exit(EXIT_FAILURE);
        }

        if (pthread_attr_setdetachstate(&new_thread_attr, PTHREAD_CREATE_DETACHED)){
            perror("Setting thread attributes failed");
            free(new_socket_desc);
            exit(EXIT_FAILURE);
        }

        if (pthread_create(&server_thread, &new_thread_attr, &thread_handling, (void*) new_socket_desc)){
            perror("Thread creation failed");
            free(new_socket_desc);
            exit(EXIT_FAILURE);
        }
    }

    shutdown(server_socket_desc, 2);
    return 0;
}
