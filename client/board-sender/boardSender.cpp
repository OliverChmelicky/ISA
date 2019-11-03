//
// Created by xchmel28 on 28.10.19.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "boardSender.h"

int boardSender::sendBoardRequest(int socket, const arguments& data) {

	std::ostringstream request;
	if (data.commandType == BOARDS) {
		request << "GET /boards HTTP/1.1" <<"\r\n"<<"";
		return write(socket, request.str().data(), request.str().length() );
    }
    else {
        switch(data.commandOperation) {
			case ADD  :
				request << "POST /boards/"<< data.boardName << " HTTP/1.1\r\nContent-Type: text/plain\r\nContent-Length: " <<data.boardName.length()<< "\r\n\r\n"<< data.boardName <<"";
				return write(socket, request.str().data(), request.str().length() );
			case DELETE  :
				request << "DELETE /boards/"<< data.boardName<< " HTTP/1.1\r\nContent-Length: 0\r\n\r\n";
				return write(socket, request.str().data(), request.str().length() );
            case LIST  :
				request << "GET /board/"<< data.boardName<< " HTTP/1.1\r\nContent-Length: 0\r\n\r\n";
				return write(socket, request.str().data(), request.str().length() );
            default : //Optional
                cout<< "Error switch board sender"<<endl;
                return -1;
        }
    }

    return -1;
}
