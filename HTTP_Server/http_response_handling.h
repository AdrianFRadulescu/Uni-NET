//
// Created by Radulescu Adrian on 29/10/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "shared_resources.h"
#include "http_request_handling.h"

// STATUS CODES

#define STATUS_CODE_200_OK                                  "HTTP/1.1 200 OK"
#define STATUS_CODE_206_PARTIAL_CONTENT                     "HTTP/1.1 206 Partial Content"
#define STATUS_CODE_302_FOUND                               "HTTP/1.1 302 Found"
#define STATUS_CODE_400_BAD_REQUEST                         "HTTP/1.1 400 Bad Request"
#define STATUS_CODE_403_FORBIDDEN                         "HTTP/1.1 403 Forbidden"
#define STATUS_CODE_404_NOT_FOUND                           "HTTP/1.1 404 Not Found"
#define STATUT_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE     "HTTP/1.1 416 Range Not Satisfiable"

// RESPONSE HEADERS

#define ACCEPT_RANGES_RESP_HD                               "Accept-Ranges: "
#define CONTENT_LENGTH_RESP_HD                              "Content-Length: "
#define CONTENT_TYPE_RESP_HD                                "Content-Type: "
#define CONTENT_ENCODING_RESP_HD                            "Content-Encoding: "
#define CONTENT_LANGUAGE_RESP_HD                            "Content-Language: en-US"
#define CONTENT_RANGE_RESP_HD                               "Content-Range: bytes "
#define CONNECTION_RESP_HD                                  "Connection: "
#define TRANSFER_ENCODING_RESP_HD                           "Transfer-Encoding: "

// RESPONSE HEADERS VALUES

#define ACCEPT_RANGES_VALUE_BYTES                           "bytes"
#define ACCEPT_RANGES_VALUE_NONE                            "none"
#define CONTENT_TYPE_VALUE_HTML                             "text/html; charset=UTF-8"
#define CONTENT_TYPE_VALUE_MULTIRANGES                      "multipart/byteranges; boundary=3d6b6a416f9b5"
#define CONNECTION_VALUE                                    "close"

// OTHER CONSTANTS

#define CRLF "\r\n"

#ifndef HTTP_SERVER_HTTP_RESPONSE_HANDLING_H
#define HTTP_SERVER_HTTP_RESPONSE_HANDLING_H

#endif //HTTP_SERVER_HTTP_RESPONSE_HANDLING_H




typedef struct http_response {

    char* status_code_;
    size_t header_count;
    http_header_t* headers_;
    char* page_;
} http_response_t;

/**
 *  Initialise response object
 */

void init_response(http_response_t*);

/**
 *  Clean the space allocated for a response object
 */

void tear_down_response(http_response_t*);

/**
 * Loads the content of a file
 * @return
 */

char* load_file_content(char[], pthread_mutex_t*);

/**
 * Prototype
 * @return
 */

char* load_response_line(char[], pthread_mutex_t*);

/**
 * Attaches a value to a header if requested
 */

void create_header(char[], char[], char[]);

void create_response(http_request_t, http_response_t*);

char* adjust_response(http_response_t);
