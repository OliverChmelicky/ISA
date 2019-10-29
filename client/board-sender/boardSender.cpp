//
// Created by olo on 28.10.19.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "boardSender.h"

int boardSender::sendBoardRequest(int socket, const arguments& data) {
    if (data.commandType == BOARDS){
		std::ostringstream request;
		request << "GET /boards HTTP/1.1" <<"\n\n"<<"";
		return write(socket, request.str().data(), request.str().length() );

    }
    else{
        switch(data.commandOperation) {
            case ADD  :

                break; //optional
            case DELETE  :

                break; //optional
            case LIST  :
                // you can have any number of case statements.
                break;
            default : //Optional
                cout<< "Error parsing arguments"<<endl;
                //som v pici
        }
    }



    return 0;
}
