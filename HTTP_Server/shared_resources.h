//
// Created by Radulescu Adrian on 30/10/2017.
//

#include <pthread.h>

#define PATH_LENGTH                 512
#define MAX_RESOURCE_PATH_LENGTH    70
#define HTML_RESOURCE_COUNT         11

#ifndef HTTP_SERVER_SHARED_RESOURCES_H
#define HTTP_SERVER_SHARED_RESOURCES_H

#endif //HTTP_SERVER_SHARED_RESOURCES_H


pthread_mutex_t resource_mutex;
char pwd[PATH_LENGTH];

typedef struct html_resource{

    char path_[MAX_RESOURCE_PATH_LENGTH];
    int permission;
} html_resource_t;


html_resource_t* html_resources_;


