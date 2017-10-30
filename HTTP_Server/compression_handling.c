//
// Created by Radulescu Adrian on 30/10/2017.
//

#include "compression_handling.h"

int deflate_file(char source_path_[], char dest_path_[]) {


    char source_arg_[70] = "";
    strncpy(source_arg_, " < ", strlen(" < "));
    strncpy(source_arg_ + strlen(source_arg_), source_path_, strlen(source_path_));

    char dest_arg_[70] = "";
    strncpy(dest_arg_, " > ", strlen(" > "));
    strncpy(dest_arg_ + strlen(dest_arg_), dest_path_, strlen(dest_path_));

    char executable_[200] = "./zlib_aux_exec";

    strncpy(executable_ + strlen(executable_), source_arg_, strlen(source_arg_));
    strncpy(executable_ + strlen(executable_), dest_arg_, strlen(dest_arg_));

    fprintf(stderr, "%s\n", executable_);
    if (system(executable_) == 0){
        fprintf(stderr, "compression success");
        return 0;
    }
    else
        return -1;
}

int gzip_file(char source_path_[], char dest_path_[]) {

    char source_arg_[70] = "";
    strncpy(source_arg_, " < ", strlen(" < "));
    strncpy(source_arg_ + strlen(source_arg_), source_path_, strlen(source_path_));

    char dest_arg_[70] = "";
    strncpy(dest_arg_, " > ", strlen(" > "));
    strncpy(dest_arg_ + strlen(dest_arg_), dest_path_, strlen(dest_path_));

    char executable_[200] = "gzip --best";

    strncpy(executable_ + strlen(executable_), source_arg_, strlen(source_arg_));
    strncpy(executable_ + strlen(executable_), dest_arg_, strlen(dest_arg_));

    fprintf(stderr, "%s\n", executable_);
    if (system(executable_) == 0){
        fprintf(stderr, "compression success");
        return 0;
    }
    else
        return -1;
}

int remove_file(char file_path_[]){

    char source_arg_[70] = "";
    strncpy(source_arg_, " ", strlen(" < "));
    strncpy(source_arg_ + strlen(source_arg_), file_path_, strlen(file_path_));

    char executable_[200] = "rm";
    strncpy(executable_ + strlen(executable_), source_arg_, strlen(source_arg_));

    if (system(executable_) == 0){
        fprintf(stderr, "file removed");
        return 0;
    }
    else
        return -1;
}
