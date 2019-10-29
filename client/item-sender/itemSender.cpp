//
// Created by olo on 28.10.19.
//

#include "unistd.h"
#include <iostream>
#include <sstream>

#include "itemSender.h"

int itemSender::sendItemRequest(int socket, const arguments& data) {
	std::ostringstream request;

	switch(data.commandOperation) {
		case ADD  :
			request << "POST /board/"<<data.boardName<<" HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: " <<data.content.length()<< "\r\n\r\n"<< data.content <<"";
			return write(socket, request.str().data(), request.str().length() );
		case DELETE  :
			request << "DELETE /board/"<<data.boardName << "/" << data.id<<" HTTP/1.1\r\nContent-Length: 0\r\n\r\n";
			return write(socket, request.str().data(), request.str().length() );
		case UPDATE  :
			request << "DELETE /board/"<<data.boardName << "/" << data.id<<" HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: " <<data.content.length()<< "\r\n\r\n"<< data.content <<"";
			return write(socket, request.str().data(), request.str().length() );
		default :
			cout<< "Error switch item sender"<<endl;
			return -1;
	}
	return -1;
}
