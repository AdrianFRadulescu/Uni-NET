cmake_minimum_required(VERSION 3.8)
project(HTTP_Server_Assignment)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES prep_http_sever.c)
add_executable(HTTP_Server_Assignment ${SOURCE_FILES})