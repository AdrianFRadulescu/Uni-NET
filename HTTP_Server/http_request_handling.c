//
// Created by Radulescu Adrian on 18/10/2017.
//

#include <stdlib.h>
#include <string.h>

#include "http_request_handling.h"

/**
 * Handles the initialisation of an http_request_t
 * @param request
 */

void init_request(http_request_t* request) {

    request ->request_line.http_version_ = NULL;
    request ->request_line.method_       = NULL;
    request ->request_line.URI_          = NULL;
    request ->header_count               = 0;

    request ->headers_ = realloc(NULL, sizeof(http_header_t) * 0);
}

void tear_down_request(http_request_t* request_){

    if (request_ ->request_line.method_)
        free(request_ ->request_line.method_);
    if (request_ ->request_line.URI_)
        free(request_ ->request_line.URI_);
    if (request_ ->request_line.http_version_)
        free(request_ ->request_line.http_version_);

    if (request_ ->headers_) {

        for (int i = 0; i < request_ ->header_count; i ++){

            if (request_ ->headers_[i].type_)
                free(request_ ->headers_[i].type_);
            if (request_ ->headers_[i].content_)
                free(request_ ->headers_[i].content_);
        }

        free(request_->headers_);
    }
}

/* Custom version of strtok
*/

char* string_strtok_r(char* str_, char* sep_, char** save_) {

    if (*save_ != NULL) {
        str_ = *save_;
    }


    char* sep_pos_ = strstr(str_, sep_);
    *save_ = sep_pos_ + strlen(sep_);
    if (sep_pos_)
        *sep_pos_ = '\0';
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

    char* result_       = realloc(NULL, 0); // the return string
    char* rest_         = *orig_;
    char* prev_rest_    = rest_;
    int result_len      = 0;
    char* tmp_;

    while ((rest_ = strstr(rest_, rep_))) {
        *rest_ = '\0';

        while (!(tmp_ = realloc(result_, result_len + sizeof(char) * strlen(prev_rest_) + sizeof(char) * strlen(with_))));

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
        while (! (tmp_ = realloc(result_, result_len + sizeof(char) * strlen(prev_rest_))));

        result_ = tmp_;
        strncpy(result_ + result_len, prev_rest_, strlen(prev_rest_));
    }

    free(*orig_);
    return result_;
}

/*
*/

void get_http_request(char* buffer, http_request_t* request, int* ok) {

    char crlf_sep_[]    = "\r\n";
    char space_sep_[]   = " ";
    char col_sep_[]     = ": ";
    char* save_         = NULL;
    char* sub_save      = NULL;
    char line[2000];
    char* aux;


    // get the request line
    fprintf(stderr, "processing http request\n");
    if ((aux = string_strtok_r(buffer, crlf_sep_, &save_)) == NULL){
        *ok = 0;
        return;
    }
    strncpy(line, aux, strlen(aux));

    // METHOD
    aux = string_strtok_r(line, space_sep_, &sub_save);
    request ->request_line.method_ = malloc(sizeof(char) * strlen(aux));
    strncpy(request ->request_line.method_, aux, strlen(aux));

    // URI
    aux = string_strtok_r(line, space_sep_, &sub_save);
    request ->request_line.URI_ = malloc(sizeof(char) * strlen(aux));
    strncpy(request ->request_line.URI_, aux, strlen(aux));

    //HTTP version
    aux = string_strtok_r(line, space_sep_, &sub_save);
    request ->request_line.http_version_ = malloc(sizeof(char) * strlen(aux));
    strncpy(request ->request_line.http_version_, aux, strlen(aux));

    // get the request headers

    int header_count = 0;
    while ((aux = string_strtok_r(NULL, crlf_sep_, &save_)) && *aux != '\0'){

        sub_save = NULL;

        while ((request ->headers_ = realloc(request ->headers_, sizeof(http_header_t) * (header_count + 1))) == NULL){
            sleep(10);
        }

        aux = string_strtok_r(aux, col_sep_, &sub_save);
        while (((request ->headers_)[header_count].type_ = malloc(sizeof(char) * strlen(aux) + 1)) == NULL) {
            sleep(10);
        }
        strncpy(request ->headers_[header_count].type_, aux, strlen(aux) + 1);


        aux = string_strtok_r(NULL, col_sep_, &sub_save);
        while ((request ->headers_[header_count].content_ = malloc(sizeof(char) * strlen(aux) + 1)) == NULL) {
            sleep(10);
        }
        strncpy(request ->headers_[header_count].content_, aux, strlen(aux) + 1);

        header_count ++;
    }

    request ->header_count = header_count;

    // get message body
}


/**
 * Loads the content of the file given by the file path
 * @param file_path_
 * @return
 */




