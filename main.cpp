
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include "parseArgs.h"
#include "enums.h"
#include "socketHelper.h"
#include "methods.h"


using namespace std;

int main(int argc, char *argv[]) {
    int port;
    if ((port = parsePort(argc, argv)) == -1) {
        errno = EFAULT;
        perror("Parse port");
        exit(EXIT_FAILURE);
    }

    //START OF SOURCE https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Create socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);


    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Binding");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    map<string, vector<string>> db;

    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
// END OF SOURCE
//
//              READING THE WHOLE MESSAGE IF IS BIGGER THAN BUFSIZE TODO
//
//        int *request = (int *)malloc(sizeof(int)*BUFSIZ);
//        int readingCicle = 1;
//        int bytesRead = 0;
//        bytesRead = read(new_socket, request, BUFSIZ);
//        if (bytesRead == BUFSIZ){
//            int lastBytesInSocket = 0;
//            while( lastBytesInSocket = read(new_socket, request, BUFSIZ) <= BUFSIZ ){
//                readingCicle += 1;
//                request = (int *)realloc(request, sizeof(int)*BUFSIZ*readingCicle);
//            }
//            bytesRead = (bytesRead * readingCicle) + lastBytesInSocket;
//        }
//
//        string daco = stringz(request);
//        char wholeRequest[BUFSIZ*readingCicle] = *request;
//      DONT FORGET TO FREE THE POINTER

        char request[BUFSIZ];
        read(new_socket, request, BUFSIZ);
        printf("%s\n", request);

        requestMethod method = getMethod(request);

        if (method == GET) {
            serveGET(request, new_socket, db);
        } else if (method == POST) {
            servePOST(request, new_socket, db);
        } else if (method == PUT) {
            servePUT();
        } else if (method == DELETE) {
            serveDELETE();
        } else {
            serveUNSUPPORTED();
        }

        cout<< "\n\n";
        for(auto elem : db)
        {
            std::cout << elem.first <<  " " << elem.second.size() << "\n";
        }

        //free(request);
        close(new_socket);
    }
    return 0;
}

