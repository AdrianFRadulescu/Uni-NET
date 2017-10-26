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

char* string_strtok_r(char* str_, char* sep_, char** save_) {

    if (*save_ != NULL) {
        str_ = *save_;
    }

    char* sep_pos_ = strstr(str_, sep_);

    *save_ = sep_pos_ + strlen(sep_);
    if (sep_pos_)
        memset(sep_pos_, 0, strlen(sep_));

    return str_;
}

void get_http_request(char* buffer, http_request_t* request, int* ok){

    char* token;
    char* sub_token;
    char crlf_sep_[] = "\r\n";
    char space_sep_[] = " ";
    char* save_ = NULL;
    char* sub_save = NULL;
    char line[2000];

    char* aux;

    // get the request line

    if ((aux = strtok_r(buffer, crlf_sep_, &save_)) == NULL){
        *ok = 0;
        return;
    }

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

    char *file_content = malloc(sizeof(char) * (size_t) file_size + 1);
    fread(file_content, (size_t) file_size, 1, file_desc_);
    file_content[file_size+1] = '\0';
    return file_content;
}

/**
 * Loads the http header contained in the given location
 * @param header_file_path
 * @return
 */

char* load_header(char header_file_path[]){

    char* msg = load_file_content(header_file_path);

    char* header = str_replace(msg, "\n", "\r\n");
    strcat(header, "\r\n");
    return header;
}

/**
 * Repla
 * @param orig
 * @param rep_
 * @param with
 * @return
 */

char *str_replace(char *orig_, char *rep_, char *with_) {

    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    size_t len_rep;  // length of rep (the string to remove)
    size_t len_with; // length of with (the string to replace rep with)
    size_t len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig_ || !rep_)
        return NULL;

    len_rep = strlen(rep_);

    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count

    if (!with_)
        with_ = "";

    len_with = strlen(with_);

    // count the number of replacements needed
    ins = orig_;
    for (count = 0; tmp = strstr(ins, rep_); count ++) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig_) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {

        ins = strstr(orig_, rep_);
        len_front = ins - orig_;
        tmp = strncpy(tmp, orig_, len_front) + len_front;
        tmp = strcpy(tmp, with_) + len_with;
        orig_ += len_front + len_rep; // move to next "end of rep"
    }

    strcpy(tmp, orig_);
    return result;
}

