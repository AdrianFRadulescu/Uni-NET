//
// Created by Radulescu Adrian on 30/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <assert.h>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384

#ifndef HTTP_SERVER_COMPRESSION_HANDLING_H
#define HTTP_SERVER_COMPRESSION_HANDLING_H

#endif //HTTP_SERVER_COMPRESSION_HANDLING_H

void z_error(int);

void deflate_file(char[], char[], int);

