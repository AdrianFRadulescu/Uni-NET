//
// Created by Radulescu Adrian on 30/10/2017.
//

#include "compression_handling.h"


void z_error(int ret) {
    perror("compression error: ");
    switch (ret) {
        case Z_ERRNO:
            if (ferror(stdin))
                perror("error reading stdin\n");
            if (ferror(stdout))
                perror("error writing stdout\n");
            break;
        case Z_STREAM_ERROR:
            perror("invalid compression level\n");
            break;
        case Z_DATA_ERROR:
            perror("invalid or incomplete deflate data\n");
            break;
        case Z_MEM_ERROR:
            perror("out of memory\n");
            break;
        default:
            perror("zlib version mismatch!\n");
    }
}


void deflate_file(char source_path_[], char dest_path_[],  int def_level){

    FILE* source_ = fopen(source_path_, "rb");
    FILE* dest_   = fopen(dest_path_, "wb");


}

