//
// Created by olo on 31.10.19.
//

#include <string>
#include <vector>
#include<map>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include "getHandler.h"

#include "../../errors/errors.h"
#include "../../structures/structures.h"
#include "../../db-access/dbAccess.h"

using namespace std;

int getHandler::serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db) {
	std::string firstLine = request.substr(0, request.find('\n'));
	URL querry = getHandler::parseURL(&firstLine);

	if (querry.type == "boards"){
		string allBoards;
		getBoards(db,allBoards);
		std::ostringstream answ;
		answ << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " <<allBoards.length()<<"\r\n\r\n"<< allBoards<<"";
		return write(socket, answ.str().data(), answ.str().length() );

	}
	else if (querry.type == "board"){
		//check ci board/name existuje a vrat
		string posts;
		if (0 != getPosts(db, querry.name,posts)){
			cout << "Board was not found\n";
			return errors::sendErrorNotFound(socket);
		}
		std::ostringstream answ;
		answ << "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " <<posts.length()<<"\r\n\r\n"<< posts<<"";
		return write(socket, answ.str().data(), answ.str().length() );
	}

	return errors::sendErrorNotFound(socket);

}

URL getHandler::parseURL(std::string *rq) {
	string unparsed = getUnparsedURL(rq);

	//parse according to '/' cound > 3 error + check first parameter if is board or boards and then react
	replace(unparsed.begin(), unparsed.end(), '/', ' ');
	vector<string> array;
	stringstream ss(unparsed);
	string temp;
	while (ss >> temp)
		array.push_back(temp);

	if (array.size() < 1 || array.size() > 2) {
		cout << "Err size\n";
		return errorStruct();
	} else if (array.front() == "boards" || array.front() == "board") {
		//  /boards
		if (array.front() == "boards") {
			if (array.size() == 1) {
				URL toReturn;
				toReturn.type = "boards";
				return toReturn;
			}
		}
		// /board/name
		if (array.front() == "board") {
			if (array.size() == 2) {
				URL toReturn;
				toReturn.type = "board";
				toReturn.name = array.back();
				return toReturn;
			}
		}
	}
	return errorStruct();
}


