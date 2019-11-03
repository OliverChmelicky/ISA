//
// Created by olo on 01.11.19.
//

#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sstream>

#include "deleteHandler.h"
#include "../../socket-helper/socketHelper.h"
#include "../../errors/errors.h"
#include "../../db-access/dbAccess.h"

using namespace std;

int deleteHandler::serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db) {
	std::string answ = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
	std::string firstLine = request.substr(0, request.find('\n'));
	URL query = deleteHandler::parseURL(&firstLine);

	if (query.type == "boards"){
		if (query.name.empty()){
			cout << "Not BOARDS err\n";
			return errors::sendErrorNotFound(socket);
		}
		if ( 0 != deleteBoard(db, query.name) ){
			cout<< "Board does not exist\n";
			return errors::sendErrorNotFound(socket);
		}
		answ = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
	}
	if (query.type == "board") {
		if (query.name.empty()){
			return errors::sendErrorNotFound(socket);
		}
		if ( 0 != deletePost(db, query.id, query.name) ){
			cout<< "Board does not exist or id is not in range\n";
			return errors::sendErrorNotFound(socket);
		}
		answ = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
	}

	return write(socket, answ.data(), answ.length() );
}

URL deleteHandler::parseURL(std::string *rq) {
		string unparsed = getUnparsedURL(rq);

		//parse according to '/' cound > 3 error + check first parameter if is board or boards and then react
		replace(unparsed.begin(), unparsed.end(), '/', ' ');
		vector<string> array;
		stringstream ss(unparsed);
		string temp;
		while (ss >> temp)
			array.push_back(temp);

		if (array.size() < 2 || array.size() > 3) {
			cout << "Err size\n";
			return errorStruct();
		} else if (array.front() == "boards" || array.front() == "board") {
			//  /boards/name
			if (array.front() == "boards") {
				if (array.size() == 2) {
					URL toReturn;
					toReturn.type = array.front();
					toReturn.name = array.back();
					return toReturn;
				}
				return errorStruct();
			}
			// /board/name/id
			if (array.front() == "board") {
				if (array.size() == 3) {
					int id = convertToInt(array.back());
					if (id < 0) {
						return errorStruct();
					}
					URL toReturn;
					toReturn.type = "board";
					toReturn.name = array[1];
					toReturn.id = id;
					return toReturn;
				}
				return errorStruct();
			}
		}
		return errorStruct();
}

