//
// Created by olo on 01.11.19.
//

#include <iostream>
#include <unistd.h>
#include <regex>

#include "../../socket-helper/socketHelper.h"
#include "postHandler.h"
#include "../../errors/errors.h"
#include "../../db-access/dbAccess.h"

using namespace std;

int postHandler::serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db) {
	std::string answ = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
	std::string firstLine = request.substr(0, request.find('\n'));
	URL query = postHandler::parseURL(&firstLine);
	int length;

	if (query.type == "boards"){
		if (query.name.empty()){
			cout << "Not BOARDS err\n";
			return errors::sendErrorNotFound(socket);
		}
		if (!contentTypeTxt(request)){
			cout << "Content type err\n";
			return errors::sendErrorBadRequest(socket);
		}
		length = contentLength(request);
		if (length <= 0){
			cout << "Length err\n";
			return errors::sendErrorBadRequest(socket);
		}

		body newBoard = readBody(request, length);
		if (newBoard.errCode != 0){
			cout << "Body read err\n";
			return errors::sendErrorBadRequest(socket);
		}
		if (query.name != newBoard.content){
			cout << "Body does not match URL\n";
			return errors::sendErrorBadRequest(socket);
		}
		const std::regex re( "[a-zA-Z0-9]*" ) ;
		std::smatch match ;
		if( !std::regex_match( query.name, match, re ) ){
			cout << "Regex err\n";
			return errors::sendErrorBadRequest(socket);
		}
		if ( 0 != createBoard(db, newBoard.content) ){
			cout<< "Board exists err\n";
			return errors::sendErrorExists(socket);
		}

		answ = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
	}
	else if (query.type == "board") {
		if (query.name.empty()){
			return errors::sendErrorNotFound(socket);
		}
		if (!contentTypeTxt(request)){
			cout << "Content type err\n";
			return errors::sendErrorBadRequest(socket);
		}
		length = contentLength(request);
		if (length <= 0){
			cout << "Length err\n";
			return errors::sendErrorBadRequest(socket);
		}

		body newPost = readBody(request, length);
		if (newPost.errCode != 0){
			cout << "Body read err\n";
			return errors::sendErrorNotFound(socket);
		}
		if ( 0 != createPost(db, query.name,newPost.content) ){
			cout<< "Board does not exist\n";
			return errors::sendErrorNotFound(socket);
		}
		answ = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
	}

	return write(socket, answ.data(), answ.length() );
}



URL postHandler::parseURL(std::string *rq) {
	string unparsed = getUnparsedURL(rq);

	//parse according to '/' cound > 3 error + check first parameter if is board or boards and then react
	replace(unparsed.begin(), unparsed.end(), '/', ' ');
	vector<string> array;
	stringstream ss(unparsed);
	string temp;
	while (ss >> temp)
		array.push_back(temp);

	if (array.size() < 1 || array.size() > 3) {
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
