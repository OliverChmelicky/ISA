//
// Created by olo on 01.11.19.
//

#include <iostream>
#include <unistd.h>
#include <regex>

#include "putHandler.h"
#include "../../socket-helper/socketHelper.h"
#include "../../errors/errors.h"
#include "../../db-access/dbAccess.h"

using namespace std;

int putHandler::serve(const string& request, int socket, map<string, vector<string>> &db){
	std::string answ = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
	std::string firstLine = request.substr(0, request.find('\n'));
	URL query = parseUrl(&firstLine);
	int length;

	if (query.type == "board") {
		if (query.name.empty()){
			return errors::sendErrorNotFound(socket);
		}
		if (!contentTypeTxt(request)){
			cout << "Content type err\n";
			return errors::sendErrorNotFound(socket);
		}
		length = contentLength(request);
		cout << "Length err\n";
		if (length <= 0){
			return errors::sendErrorBadRequest(socket);
		}

		body newPost = readBody(request, length);
		if (newPost.errCode != 0){
			cout << "Body read err\n";
			return errors::sendErrorNotFound(socket);
		}

		if ( 0 != updatePost(db, query.id, newPost.content,query.name) ){
			cout<< "Board does not exist\n";
			return errors::sendErrorNotFound(socket);
		}
		answ = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
	}

	return write(socket, answ.data(), answ.length() );
};


URL putHandler::parseUrl(string *rq) {
	string unparsed = getUnparsedURL(rq);

	replace(unparsed.begin(), unparsed.end(), '/', ' ');
	vector<string> array;
	stringstream ss(unparsed);
	string temp;
	while (ss >> temp)
		array.push_back(temp);

	if (array.size() != 3) {
		cout << "Err size\n";
		return errorStruct();
	}
		// /board/name/id --> in getTypeThree
	else if (array.front() == "board") {
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


