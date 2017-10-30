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

    FILE* file_desc_ = fopen(file_path_, "r");
    fseek(file_desc_, 0, SEEK_END);
    long file_size = ftell(file_desc_);
    fseek(file_desc_, 0, SEEK_SET);

    char *file_content_ = malloc(sizeof(char) * (size_t) file_size + 1);
    fread(file_content_, (size_t) file_size, 1, file_desc_);
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
    strncpy(response_ ->headers_[response_ ->header_count].type_, header_type_, strlen(header_type_) + 1);
    response_ ->headers_[response_ ->header_count].type_[strlen(header_type_)] = '\0';

    response_ ->headers_[response_ ->header_count].content_ = malloc(sizeof(char) * strlen(header_value_));
    strncpy(response_ ->headers_[response_ ->header_count].content_, header_value_, strlen(header_value_) + 1);
    response_ ->headers_[response_ ->header_count].content_[strlen(header_value_)] = '\0';

    response_ ->header_count ++;

}

static int check_URI(char* URI_){
    fprintf(stderr, "%s\n", URI_);

    for (int i = 0; i < HTML_RESOURCE_COUNT; i ++){
        if (strcmp(html_resources_))
    }

    return 0;
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

        response_ ->status_code_ = malloc(sizeof(char) * strlen(STATUS_CODE_200_OK) + 1);
        strncpy(response_ ->status_code_, STATUS_CODE_200_OK, strlen(STATUS_CODE_200_OK) + 1);
        fprintf(stderr, "stat_code1 = %s\n", response_ ->status_code_);
        add_header(response_, CONTENT_TYPE_RESP_HD, CONTENT_TYPE_VALUE_HTML);
        response_ ->page_ = load_file_content(strcat(pwd, "html_pages/index.html"), &resource_mutex);

        char buffer_[256] = "";

        sprintf(buffer_, "%zu", strlen(response_ ->page_));

        add_header(response_, CONTENT_LENGTH_RESP_HD, buffer_);

        return;
    }

    else {
        //check if URI is valid
        if (check_URI(request.request_line.URI_) == 1) {
            // tried to access forbidden page


        }
        else {

        }
    }

}

char* adjust_response(http_response_t response){

    char crlf_[]    = "\r\n";
    char* buffer_   = malloc(sizeof(char) * (strlen(response.status_code_) + 1));
    // add the status code
    strncpy(buffer_, response.status_code_, strlen(response.status_code_) + 1);
    fprintf(stderr, "stat_code = %s\n", response.status_code_);
    buffer_ = realloc(buffer_, sizeof(char) * (strlen(buffer_) + strlen(crlf_) + 1));
    strncpy(buffer_ + strlen(buffer_), crlf_, strlen(crlf_) + 1);

    for (int i = 0; i < response.header_count; i ++) {
        buffer_ = realloc(buffer_, sizeof(char) * (strlen(buffer_) + strlen(response.headers_[i].type_) + strlen(response.headers_[i].content_) + strlen(crlf_) + 1));

        strncpy(buffer_ + strlen(buffer_), response.headers_[i].type_, strlen(response.headers_[i].type_) + 1);
        strncpy(buffer_ + strlen(buffer_), response.headers_[i].content_, strlen(response.headers_[i].content_) + 1);
        strncpy(buffer_ + strlen(buffer_), crlf_, strlen(crlf_) + 1);
    }

    buffer_ = realloc(buffer_, sizeof(char) * (strlen(buffer_) + strlen(crlf_) + 1));
    strncpy(buffer_ + strlen(buffer_), crlf_, strlen(crlf_) + 1);

    // copy message body;

    buffer_ = realloc(buffer_, sizeof(char) * (strlen(buffer_) + strlen(response.page_) + 1));
    strncpy(buffer_ + strlen(buffer_), response.page_, strlen(response.page_) + 1);

    return buffer_;
}
