//
// Created by olo on 31.10.19.
//

#include "base.h"
#include "../../socket-helper/socketHelper.h"

using namespace std;

std::string Base::getUnparsedURL(std::string *rq) {
	string request = *rq;

	string preprocessed = request.substr(request.find("/"), request.length());

	string url = preprocessed.substr(1, preprocessed.find(" "));
	if (url.length() < 1) {
		return "";
	}
	url.pop_back(); //get rid of " " character

	//condition for next operations of getting last character
	if (url.length() <= 2) {
		return "";
	}
	string lastLetter = url.substr(url.length() - 1, url.length()); //last letter might be '/'
	if (lastLetter == "/") {
		url.pop_back();
	}
	return url;
}

body Base::readBody(std::string request, int length) {
	size_t pos = 0;
	std::string bodyString;
	string delimiter = "\r\n\r\n";
	struct body bodyToReturn;

	pos = request.find(delimiter);
	pos += 4;
	if (pos > request.size()){
		bodyToReturn.errCode = 1;
		return bodyToReturn;
	}
	bodyString = request.substr(pos, request.size());

	if (bodyString.length() < length)
		length = bodyString.length();
	bodyString = trim(bodyString.substr(0, length));

	bodyToReturn.errCode = 0;
	bodyToReturn.content = bodyString;

	if (bodyString.empty())
		bodyToReturn.errCode = 1;

	return bodyToReturn;
}
