//
// Created by Radulescu Adrian on 18/10/2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>

#ifndef HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H
#define HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H

#endif //HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H

/**
 *  Represents a request line from an http request
 */

typedef struct http_request_line{

    char* request_method;
    char* request_URI;
    char* request_http_version;

} http_request_line_t;

/**
 *  Represents an http request
 */

typedef struct http_request_block{

    http_request_line_t request_line;
    int request_header_count;
    int request_header_available;
    char** request_headers;
    char* request_message;

} http_request_t;


void init_request(http_request_t*);

void tear_down_request(http_request_t*);

char* string_strtok_r(char*, char*, char**);

/**
 * Parse a given buffer and return a http_request_t out of it
 * @param buffer
 * @return
 */

void get_http_request(char*, http_request_t*, int*);

/**
 * Load the content of a file which is to be sent with the request
 * @return
 */

char* load_file_content(char[]);

char* load_header(char[]);

char *str_replace(char *, char*, char*);