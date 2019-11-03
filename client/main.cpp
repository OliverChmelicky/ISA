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
#include "item-sender/itemSender.h"
#include "../server/structures/structures.h"

using namespace std;

int connectToServer(int& socket, struct sockaddr_in& server, const string& host, int port);
body readBody(const string& answer);

string convertToString(char* a, int size)
{
	int i;
	string s;
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

int main(int argc, char *argv[]) {
    arguments arg = parser::parse(argc,argv);
    //checkni id ci je cislo a prejdi regexom boarName

    if (arg.errorCode != 0 ){
        cout<< "Error parsing arguments\n";
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
			n = itemSender::sendItemRequest(sock,arg);
			break;
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

	int size = sizeof(buffer) / sizeof(char);
	string bufferS = convertToString(buffer, size);

	body answer;
    answer = readBody(bufferS);

	if (answer.errCode == 0){
		cout<<answer.content<<endl;
	}
//	else{
//		cout<<"Error reading body:\n";
//		printf("%s\n", buffer);
//		cout<<"von";
//	}

    close(sock);

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

	// Write resolved IP address of a server to the address structure
	memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

	// Connect to the remote server
	int res = connect(socket, (struct sockaddr*) &server, sizeof(server));
	if (res < 0)
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		return -1;
	}

	return 0;
}

body readBody(const string& answer) {
	int from = answer.find(' ');
	int to = answer.find('\r');
	std::string headder = answer.substr(answer.find(' '), to-from);

	std::string bodyString;
	string delimiter = "\r\n\r\n";
	struct body bodyToReturn;

	size_t pos = 0;
	pos = answer.find(delimiter);
	pos += 4;
	if (pos > answer.size()){
		bodyToReturn.errCode = 1;
		return bodyToReturn;
	}

	bodyToReturn.errCode = 0;
	bodyToReturn.content = headder.append("\n" + answer.substr(pos, answer.size()));

	return bodyToReturn;
}