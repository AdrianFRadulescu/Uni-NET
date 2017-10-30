//
// Created by Radulescu Adrian on 29/10/2017.
//

#include "http_response_handling.h"


void init_response(http_response_t* response_) {

    response_ ->status_code_     = NULL;
    response_ ->page_  = NULL;
    response_ ->header_count    = 0;

    while (!(response_ ->headers_ = realloc(NULL, sizeof(http_header_t) * 0))){
        sleep(10);
    }
}

void tear_down_response(http_response_t* response_) {

    if (response_ ->headers_) {

        for (int i = 0; i < response_ ->header_count; i ++){

            if (response_ ->headers_[i].type_)
                free(response_ ->headers_[i].type_);
            if (response_ ->headers_[i].content_)
                free(response_ ->headers_[i].content_);
        }
        free(response_->headers_);
    }

    if (response_ ->status_code_)
        free(response_ ->status_code_);

    if (response_ ->page_)
        free(response_ ->page_);
}

char* load_file_content(char file_path_[], pthread_mutex_t* mutex_) {

    pthread_mutex_lock(mutex_);
    FILE* file_desc_ = fopen(file_path_, "r");
    fseek(file_desc_, 0, SEEK_END);
    long file_size = ftell(file_desc_);
    fseek(file_desc_, 0, SEEK_SET);

    char *file_content_ = malloc(sizeof(char) * (size_t) file_size + 1);
    fread(file_content_, (size_t) file_size, 1, file_desc_);
    pthread_mutex_unlock(mutex_);
    file_content_[file_size+1] = '\0';
    fprintf(stderr, "load_file %p\n", (void*) file_content_);
    return file_content_;
}

/**
 * Loads the http header contained in the given location
 * @param header_file_path
 * @return
 */

void create_header(char header_[], char header_type_[], char header_value_[]) {
    strncpy(header_, header_type_, strlen(header_type_));
    strncpy(header_ + strlen(header_type_), header_value_, strlen(header_value_));
    strncpy(header_ + strlen(header_type_) + strlen(header_value_), "\r\n", strlen("\r\n"));
}

char* load_response_line(char file_path_[], pthread_mutex_t* mutex_) {
    return strcat(load_file_content(file_path_, mutex_), "\r\n");
}

static void add_header(http_response_t* response_, char header_type_[], char header_value_[]){



    response_ ->headers_ = realloc(response_ ->headers_, sizeof(http_header_t) * (response_ ->header_count + 1));

    response_ ->headers_[response_ ->header_count].type_ = malloc(sizeof(char) * strlen(header_type_));
    strncpy(response_ ->headers_[response_ ->header_count].type_, header_type_, strlen(header_type_));

    response_ ->headers_[response_ ->header_count].content_ = malloc(sizeof(char) * strlen(header_value_));
    strncpy(response_ ->headers_[response_ ->header_count].content_, header_value_, strlen(header_value_));

    response_ ->header_count ++;

}

static void check_URI(char* URI_){
    fprintf(stderr, "%s\n", URI_);
}

void create_response(http_request_t request, http_response_t* response_){

    // check request to see if it's valid

    // check HTTP version
    if (strncmp(SERVER_HTTP_VERSION, request.request_line.http_version_, strlen(SERVER_HTTP_VERSION)) != 0) {

        fprintf(stderr, "bad request, bad HTTP version\n");

        response_ ->status_code_ = malloc(sizeof(char) * strlen(STATUS_CODE_400_BAD_REQUEST));
        strncpy(response_ ->status_code_, STATUS_CODE_400_BAD_REQUEST, strlen(STATUS_CODE_400_BAD_REQUEST));
        response_ ->page_ = load_file_content(strcat(pwd, "html_pages/error_400_bad_request.html"), &resource_mutex);
        return;
    }

    // check request method
    if (strncmp(GET, request.request_line.method_, strlen(GET)) != 0 && strncmp(HEAD, request.request_line.method_, strlen(HEAD)) != 0) {

        fprintf(stderr, "bad request, bad HTTP version\n");

        response_ ->status_code_ = malloc(sizeof(char) * strlen(STATUS_CODE_400_BAD_REQUEST));
        strncpy(response_ ->status_code_, STATUS_CODE_400_BAD_REQUEST, strlen(STATUS_CODE_400_BAD_REQUEST));

        add_header(response_, CONNECTION_RESP_HD, CONNECTION_VALUE);

        response_ ->page_ = load_file_content(strcat(pwd, "html_pages/error_400_bad_request.html"), &resource_mutex);
        return;
    }

    // check URI

    // special case
    if (strlen(request.request_line.URI_) == 1 && *request.request_line.URI_ == '/'){

        response_ ->status_code_ = malloc(sizeof(char) * strlen(STATUS_CODE_200_OK));
        strncpy(response_ ->status_code_, STATUS_CODE_200_OK, strlen(STATUS_CODE_200_OK));
        response_ ->page_ = load_file_content(strcat(pwd, "html_pages/index.html"), &resource_mutex);
    }

    else{
        //check if URI is valid
        check_URI(request.request_line.URI_);
    }

}

