//
// Created by Radulescu Adrian on 18/10/2017.
//
#include "connection_handling.h"

#define BUFFER_LENGTH (8*1024)


void pwd_adjust(const char* str_, char pwd[]){

    realpath(str_, pwd);
    char* it_ = pwd + strlen(pwd);
    while (*(--it_) != '/');
    *(it_+1) = '\0';
}

/**
 * Puts given address in human readable format
 * @param addr the given IPV6/IPV4 address
 * @param addr_len the length of the given address
 * @param buffer the string which stores the human readable address
 * @param buffer_size the size of the buffer in bytes
 * @return
 */

char* create_printable_address(const struct sockaddr_in6 *const address,
                               char *const buffer,
                               const size_t buffer_size) {

    /**
     * Create a buffer to hold the printable address
     */
    char printable[INET6_ADDRSTRLEN];

    if(inet_ntop(address->sin6_family, &(address->sin6_addr),
                 printable, sizeof(printable)) == printable) {

        char msg[] = "Client with address ";
        strncpy(buffer, msg, strlen(msg));
        strncpy(buffer + strlen(msg), printable, strlen(printable));
        fprintf(stderr, "%s\n", buffer);

    } else {

        perror("Could not parse the address.");
        //char msg[] = "Address cannot be parsed";
        //strncmp(buffer,msg, sizeof(msg));
    }

    return buffer;
}




/**
 * Reads and parses a request from a client and then sends the appropriate response
 * @param client_desc client descriptor
 * @param client_info contains the
 */

static void handle_client_request(const int client_desc, const char* client_info){

    char buffer[BUFFER_LENGTH];

    // read request from client
    read(client_desc, buffer, BUFFER_LENGTH);
    // send(client_desc, client_info, strlen(client_info) * sizeof(char),0);

    fprintf(stderr, "RECEIVEDL:\n%s", buffer);

    http_request_t client_request;
    int success = 1;
    init_request(&client_request);

    // parse the buffer to get the http request
    get_http_request(buffer, &client_request, &success);

    if (!success){
        perror("error on parsing http request");
        return;
    }

    fprintf(stderr, "RECEIVED:\n\n");
    fprintf(stderr, "%c\n", *client_request.request_line.request_method);
    fprintf(stderr, "%d\n", strncmp("GET", client_request.request_line.request_method, 3));
    fprintf(stderr, "%s\n", client_request.request_line.request_method);


    fprintf(stderr, "%s\n", client_request.request_line.request_http_version);
    fprintf(stderr, "%s\n", client_request.request_line.request_URI);

    //char* msg_response_line_ = load_response_line(strcat(pwd, ));

    /*
    char* msg_header_ = load_header("/Users/adrian_radulescu1997/Documents/Uni-Courses/III/NET/Uni-NET/HTTP_Server/http_headers/", &resource_mutex);
    //msg_header_ = strcat(msg_header_, );


    char*  msg_content_ = load_file_content("/Users/adrian_radulescu1997/Documents/Uni-Courses/III/NET/Uni-NET/HTTP_Server/html_pages/erorr_page.html", &resource_mutex);

    // attach content length
    size_t message_content_length = sizeof(char) *  + strlen(msg_content_);

    char* content_header_annex_ = malloc(sizeof(char)*256);

    sprintf(content_header_annex_, "Content-Length: %zu\r\n", message_content_length);

    content_header_annex_ = realloc(content_header_annex_, sizeof(char) * strlen(content_header_annex_));

    msg_header_ = strcat(msg_header_, content_header_annex_);
    msg_header_ = strcat(msg_header_, "\r\n");


    send(client_desc, msg_header_, sizeof(char) * strlen(msg_header_), 0);
    send(client_desc, msg_content_, sizeof(char) * strlen(msg_content_), 0);
    // free the dynamically allocated headers

    fprintf(stderr,"SENDING:\n %s%s",msg_header_, msg_content_);

    tear_down_request(&client_request);
    free(msg_header_);
    free(msg_content_);
    */
 }


void cleanup_thread(void* th_data){

}

/**
 * Runs the process of a client thread
 * @param th_data_
 * @return
 */

void* thread_process(void* th_data_){

    thread_control_block_t *tcb_ = (thread_control_block_t*) th_data_;
    char buffer[INET6_ADDRSTRLEN];
    char* printable_addr;

    printable_addr = create_printable_address(&(tcb_->client_addr),  buffer, sizeof(buffer));

    handle_client_request(tcb_ ->client_desc, buffer);

    //free(printable_addr);
    free(th_data_);
    pthread_exit(EXIT_SUCCESS);
}

/**
 * Creates a client thread
 * @param server_socket_desc the socket used by the server
 * @return
 */

int create_client_thread(const int server_socket_desc){

    thread_control_block_t* tcb_ = malloc(sizeof(thread_control_block_t));

    if (!tcb_){
        perror("malloc");
        return EXIT_FAILURE;
    }

    if ((tcb_ ->client_desc = accept(server_socket_desc, (struct sockaddr*)&(tcb_ ->client_addr), &(tcb_ ->client_addr_size))) < 0){
        perror("accept");
        free(tcb_);
        return EXIT_FAILURE;
    }

    pthread_t server_thread;
    pthread_attr_t server_thread_attr;

    // create separate thread for processing
    if (pthread_attr_init(&server_thread_attr)){
        perror("Creating initial thread attributes failed");
        free(tcb_);
        return EXIT_FAILURE;
    }

    if (pthread_attr_setdetachstate(&server_thread_attr, PTHREAD_CREATE_DETACHED)){
        perror("Setting thread attributes failed");
        free(tcb_);
        return EXIT_FAILURE;
    }

    if (pthread_create(&server_thread, &server_thread_attr, &thread_process, (void*) tcb_)){
        perror("Thread creation failed");
        free(tcb_);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


