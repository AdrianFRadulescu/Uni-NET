//
// Created by Radulescu Adrian on 18/10/2017.
//

#include <stdlib.h>
#include <string.h>
#include <zconf.h>

#include "http_request_handling.h"

/**
 * Handles the initialisation of an http_request_t
 * @param request
 */

void init_request(http_request_t* request){

    request ->request_header_count = 0;
    request ->request_header_available = 3;
    request ->request_headers = malloc(request ->request_header_available * sizeof(char*));

    if (!request ->request_headers){
        perror("request malloc");
        exit(EXIT_FAILURE);
    }
}

void tear_down_request(http_request_t* request){

    free(request ->request_headers);
}

/* Custom version of strtok
*/

char* string_strtok_r(char* str_, char* sep_, char** save_) {

    if (*save_ != NULL) {
        str_ = *save_;
    }

    char* sep_pos_ = strstr(str_, sep_);
    fprintf(stderr, "here2\n");
    *save_ = sep_pos_ + strlen(sep_);
    if (sep_pos_)
        memset(sep_pos_, 0, strlen(sep_));

    return str_;
}


/**
 * Repla
 * @param orig
 * @param rep_
 * @param with
 * @return
 */

char* str_replace(char** orig_, char* rep_, char* with_) {

    char* result_ = realloc(NULL, 0); // the return string
    char* rest_ = *orig_;
    char* prev_rest_ = rest_;
    char* tmp_;
    int result_len = 0;

    while ((rest_ = strstr(rest_, rep_))) {
        *rest_ = '\0';

        tmp_ = realloc(result_, sizeof(char) * strlen(prev_rest_) + sizeof(char) * strlen(with_));

        while (!(tmp_ = realloc(result_, sizeof(char) * strlen(prev_rest_)))) {
            sleep(10);
        }

        result_ = tmp_;
        strncpy(result_ + result_len, prev_rest_, strlen(prev_rest_));
        strncpy(result_ + result_len + strlen(prev_rest_), with_, strlen(with_));
        result_len += strlen(prev_rest_) + strlen(with_);
        *rest_ = *rep_;
        rest_ += strlen(rep_);
        prev_rest_ = rest_;
    }

    //reached last substring of the orignal string
    if (prev_rest_ && strlen(prev_rest_) != 0) {
        while (! (tmp_ = realloc(result_, sizeof(char) * strlen(prev_rest_)))) {
            sleep(10);
        }

        result_ = tmp_;
        strncpy(result_ + result_len, prev_rest_, strlen(prev_rest_));
        result_len += strlen(prev_rest_);
    }

    //free(*orig_);
    return result_;
}

/*
*/

void get_http_request(char* buffer, http_request_t* request, int* ok){

    //char* token;
    //char* sub_token;
    char crlf_sep_[] = "\r\n";
    char space_sep_[] = " ";
    char* save_ = NULL;
    char* sub_save = NULL;
    char line[2000];

    char* aux;

    // get the request line
    fprintf(stderr, "processing http reqest\n");
    if ((aux = string_strtok_r(buffer, crlf_sep_, &save_)) == NULL){
        *ok = 0;
        return;
    }
    fprintf(stderr, "here1\n");
    strncpy(line, aux, strlen(aux));

    // METHOD
    request ->request_line.request_method = string_strtok_r(line, space_sep_, &sub_save);
    // URI
    request ->request_line.request_URI = string_strtok_r(NULL, space_sep_, &sub_save);
    //HTTP version
    request ->request_line.request_http_version = string_strtok_r(NULL, space_sep_, &sub_save);

    // get the request headers




    // get message body
}


/**
 * Loads the content of the file given by the file path
 * @param file_path
 * @return
 */

char* load_file_content(char file_path[]){

    FILE* file_desc_ = fopen(file_path, "r");
    fseek(file_desc_, 0, SEEK_END);
    long file_size = ftell(file_desc_);
    fseek(file_desc_, 0, SEEK_SET);

    char *file_content_ = malloc(sizeof(char) * (size_t) file_size + 1);
    fread(file_content_, (size_t) file_size, 1, file_desc_);
    file_content_[file_size+1] = '\0';
    fprintf(stderr, "load_file %p\n", file_content_);
    return file_content_;
}

/**
 * Loads the http header contained in the given location
 * @param header_file_path
 * @return
 */

char* load_header(char header_file_path[]){

    char* header_ = load_file_content(header_file_path);

    header_ = str_replace(&header_, "\n", "\r\n");
    fprintf(stderr, "load_header %p\n", header_);
    header_ = strcat(header_, "\r\n");
    fprintf(stderr, "load_header %p\n", header_);
    return header_;
}



