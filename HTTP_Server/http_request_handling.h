//
// Created by Radulescu Adrian on 18/10/2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


// METHODS

#define GET                     "GET"
#define HEAD                    "HEAD"

// HTTP VERSION
#define SERVER_HTTP_VERSION            "HTTP/1.1"

// REQUEST HEADERS

#define CONTENT_LENGTH_REQ_HD   "Content-Length: "
#define CONTENT_TYPE_REQ_HD     "Content-Type: text/html; charset=UTF-8"
#define AGE_REQ_HD              "Age: "
#define ACCEPT_ENCODING_REQ_HD  "Accept-Encoding: "
#define ACCEPT_RANGES_REQ_HD    "Accept-Ranges: "

#ifndef HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H
#define HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H

#endif //HTTP_BASIC_SERVER_HTTP_REQUEST_HANDLING_H

/**
 *  Represents a request line from an http request
 */

typedef struct http_request_line {

    char* method_;
    char* URI_;
    char* http_version_;

} http_request_line_t;

typedef struct http_header{
    char* type_;
    char* content_;
} http_header_t;


/**
 *  Represents an http request
 */

typedef struct http_request_block {

    http_request_line_t request_line;
    int header_count;
    http_header_t* headers_;

} http_request_t;


void init_request(http_request_t*);

void tear_down_request(http_request_t*);

char* string_strtok_r(char*, char*, char**);

char* str_replace(char**, char*, char*);

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

