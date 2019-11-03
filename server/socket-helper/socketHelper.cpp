//
// Created by olo on 19.10.19.
//

#include <string>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include "socketHelper.h"

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";


int serveUnsupported(int socket){
	std::string answ = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
	return write(socket, answ.data(), answ.length());
}

string ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const std::string &s) {
    return rtrim(ltrim(s));
}


int convertToInt(string toConvert) {
    int i = 0;
    for (char c: toConvert) {
        if (c >= '0' && c <= '9') {
            i = i * 10 + (c - '0');
        } else {
            return -1;
        }
    }

    return std::atoi(toConvert.c_str());
}

string removeSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

bool contentTypeTxt(string request) {
    size_t pos = 0;
    std::string header;
    string delimiter = "\n";
    bool occuredOnce = false;

    cout<<"jebem"<<endl;
    int i = 0;

    while ((pos = request.find(delimiter)) != std::string::npos) {
    	i ++;
        header = request.substr(0, pos);
        header = removeSpaces(header);
        std::string headerName = header.substr(0, 13);

		cout<< "headder is:"<<endl;
		cout<< header<<endl;
		cout<<headerName<<endl;

        if (headerName == "Content-Type:") {                //can be many "Content-Type:text/plain" but no other
        	cout<<"SOM DNU\n\n";
			for (int i = 0; i < header.size(); i++) {
				cout << int(header[i]) << endl;
			}
            if (header != "Content-Type:text/plain\r") {
                return false;
            }
            occuredOnce = true;
        }

        request.erase(0, pos + delimiter.length());
    }

    cout<<i<<endl;
    return occuredOnce;
}

int contentLength(string request) {
    size_t pos = 0;
    std::string header;
    string delimiter = "\n";
    bool occuredOnce = false;
    int number = -1;
    string numberStr;

    while ((pos = request.find(delimiter)) != std::string::npos) {
        header = request.substr(0, pos);
        header = removeSpaces(header);
        std::string headerName = header.substr(0, 15);

        if (header.size() >= 15)
            numberStr = header.substr(15, request.length());
        else {
            request.erase(0, pos + delimiter.length());
            continue;
        }

        if (headerName == "Content-Length:") {                //can be many "Content-Length: x" with the same number
            if (numberStr.length() < 1) { //last char is "\n"
                return -1;
            }
            numberStr = numberStr.substr(0, numberStr.size() - 1);

            if (!occuredOnce) {
                number = convertToInt(numberStr);
                occuredOnce = true;
            } else {
                if (number != convertToInt(numberStr))
                    return -1;
            }
        }

        request.erase(0, pos + delimiter.length());
    }

    return number;
}

int checkProtocol(const string& request){
	string delimiter = "\r";
	std::string headder = request.substr(0, request.find(delimiter));

	string preprocessed = headder.substr(headder.find(' '), headder.length());

	string protocol = preprocessed.substr(preprocessed.find(" H"), preprocessed.length());

	if (protocol == " HTTP/1.1"){
		return 0;
	}

	return -1;
}

requestMethod getMethod(const string& request) {
    if (request.size() >= 6) {
        string method = request.substr(0, request.find(' '));
        int i = method.compare("GET");
        if (i == 0) {
            return GET;
        }
        i = method.compare("POST");
        if (i == 0) {
            return POST;
        }
        i = method.compare("PUT");
        if (i == 0) {
            return PUT;
        }
        i = method.compare("DELETE");
        if (i == 0) {
            return DELETE;
        }
        return UNKNOWN;
    }
    return UNKNOWN;
}