//
// Created by xchmel28 on 28.10.19.
//

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <cstring>

#include "enums/enums.h"
#include "parser/parser.h"
#include "board-sender/boardSender.h"

using namespace std;

int connectToServer(int& socket, struct sockaddr_in& server, const string& host, int port);

int main(int argc, char *argv[]) {
    arguments arg = parser::parse(argc,argv);
    //checkni id ci je cislo a prejdi regexom boarName

    if (arg.errorCode != 0 ){
        cout<< "ERROR:: ";
        cout<<arg.errorCode <<endl;
        return 1;
    }



// Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    struct sockaddr_in server;
	bzero( &server, sizeof( server ) );

	if ( connectToServer(sock,server,arg.host, arg.port) != 0){
		cout<<"Error connecting to server"<<endl;
		return -1;
	}

	int n;

	switch(arg.commandType) {
		case BOARDS :
			n = boardSender::sendBoardRequest(sock,arg);
			break; //optional
		case BOARD  :
			n = boardSender::sendBoardRequest(sock,arg);
			break; //optional
		case ITEM:

		default : //Optional
			cout<<"Error switch in main!";
			return -1;
	}

	if (n < 0) {
		cout<<"Error sending request!";
		return -1;
	}

    char buffer[BUFSIZ];

    bzero(buffer,BUFSIZ);
    n = read(sock, buffer, BUFSIZ);
    if (n < 0){
        cout<<"ERROR reading socket\n"<<endl;
        return -1;
    }
    printf("%s\n", buffer);

    close(sock);

    //end of source

//
//    cout<< "HOSTNAME: ";
//    cout<<arg.host <<endl;
//
//    cout<< "PORT: ";
//    cout<<arg.port <<endl;
//
//    cout<< "TYPE: ";
//    cout<<arg.commandType <<endl;
//
//    cout<< "METHOD: ";
//    cout<<arg.commandOperation <<endl;
//
//    cout<< "CONTENT: ";
//    cout<<arg.content.data() <<endl;
//
//    cout<< "ID: ";
//    cout<<arg.id <<endl;
//
//    cout<< "BOARDNAME: ";
//    cout<<arg.boardName.data() <<endl;
//
//
//

    return 0;

}

int connectToServer(int& socket, struct sockaddr_in& server, const string& hostDomain, int port){
	char cstr[hostDomain.length() + 1];
	char* peerHost = strcpy(cstr, hostDomain.c_str());;

	// Resolve server address (convert from symbolic name to IP number)
	struct hostent *host = gethostbyname(peerHost);
	if (host == NULL)
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return -1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	// Print a resolved address of server (the first IP of the host)
	std::cout << "server address = " << (host->h_addr_list[0][0] & 0xff) << "." <<
			  (host->h_addr_list[0][1] & 0xff) << "." <<
			  (host->h_addr_list[0][2] & 0xff) << "." <<
			  (host->h_addr_list[0][3] & 0xff) << ", port " <<
			  static_cast<int>(port) << std::endl;

	// Write resolved IP address of a server to the address structure
	memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

	// Connect to the remote server
	int res = connect(socket, (struct sockaddr*) &server, sizeof(server));
	if (res < 0)
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return -1;
	}

	std::cout << "Connected!" << std::endl;
	return 0;

}