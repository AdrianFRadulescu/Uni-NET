#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "listening_socket.h"
#include "connection_handling.h"
#include "compression_handling.h"

int main(const int argc, const char* argv[]) {


    //deflate_file("html_pages/error_400_bad_request.html", "./tmp/aux1112.z");
    ///Users/adrian_radulescu1997/Documents/Uni-Courses/III/NET/Uni-NET/HTTP_Server/html_pages/index.html
    // adjust html resources

    html_resource_t html_resource1_[] = {

        {"html_pages/images/AEGYPTUS.jpg", 1},
        {"html_pages/images/afghanistan-mountains.jpg",1},
        {"html_pages/images/Greco-Bactrian_Kingdom_2nd_century_BC.jpg",1},
        {"html_pages/images/llama_man.jpg", 1},
        {"html_pages/erorr_page.html", 1},
        {"html_pages/error_400_bad_request.html", 1},
        {"html_pages/error_404_not_found.html", 1},
        {"html_pages/Forbidden_Page.html", 0},
        {"html_pages/index.html", 1},
        {"html_pages/Manifesto.html", 1},
        {"html_pages/The True Origin.html", 1}
    };

    html_resources_ = malloc(sizeof(html_resource_t) * HTML_RESOURCE_COUNT);

    memcpy(html_resources_, html_resource1_, sizeof(html_resource1_));

    // get port
    char* end_;
    uint16_t port;

     if (argc == 2) {
         port = (uint16_t) strtol(argv[1], &end_, 10);
         // adjust path of the application
         pwd_adjust(argv[0], pwd);
    } else {
         port = 3349;
         strcpy(pwd, "/Users/adrian_radulescu1997/Documents/Uni-Courses/III/NET/Uni-NET/HTTP_Server/");
    }

    if (1024 >= port || port > pow(2,31)) {
        perror("Incorrect port number");
        exit(EXIT_FAILURE);
    }


    int server_socket_desc = create_listening_socket(port, 10);

    while (create_client_thread(server_socket_desc) >= 0);

    shutdown(server_socket_desc, 2);

    free(html_resources_);
    //*/
    return 0;
}

