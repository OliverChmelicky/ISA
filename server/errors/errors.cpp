//
// Created by olo on 31.10.19.
//

#include <string>
#include <unistd.h>
#include "errors.h"

int errors::sendErrorNotFound(int socket) {
	std::string answ = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
	return  write(socket, answ.data(), answ.length());
}

int errors::sendErrorBadRequest(int socket) {
	std::string answ = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
	return write(socket, answ.data(), answ.length() );
}

int errors::sendErrorExists(int socket){
	std::string answ = "HTTP/1.1 409 Conflict\r\nContent-Length: 0\r\n\r\n";
	return write(socket, answ.data(), answ.length() );
}