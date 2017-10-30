//
// Created by Radulescu Adrian on 30/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#define CHUNK 16384

#ifndef HTTP_SERVER_COMPRESSION_HANDLING_H
#define HTTP_SERVER_COMPRESSION_HANDLING_H

#endif //HTTP_SERVER_COMPRESSION_HANDLING_H

int deflate_file(char[], char[]);

int gzip_file(char[], char[]);

int remove_file(char[]);