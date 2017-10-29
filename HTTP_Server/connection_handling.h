//
// Created by Radulescu Adrian on 18/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include <memory.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>


#include "http_request_handling.h"

#define PATH_LENGTH 512

#ifndef HTTP_BASIC_SERVER_CONNECTION_HANDLING_H
#define HTTP_BASIC_SERVER_CONNECTION_HANDLING_H
#endif //HTTP_BASIC_SERVER_CONNECTION_HANDLING_H


pthread_mutex_t resource_mutex;
char pwd[PATH_LENGTH];

typedef struct thread_control_block{
    int client_desc;
    struct sockaddr_in6 client_addr;
    socklen_t client_addr_size;
} thread_control_block_t;

void pwd_adjust(const char*, char[]);

void* thread_process(void*);

int create_client_thread(int);



