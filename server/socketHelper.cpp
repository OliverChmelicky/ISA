//
// Created by olo on 19.10.19.
//

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include<map>
#include "socketHelper.h"

using namespace std;

const std::string WHITESPACE = " \n\r\t\f\v";

URL errorStruct() {
    URL toReturn;
    toReturn.type = "";
    toReturn.name = "";
    toReturn.id = -1;
    return toReturn;
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


requestMethod getMethod(string request) {
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

URL getParsedURL(string *rq) {
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
        //  /boards  /boards/name
        if (array.front() == "boards") {
            if (array.size() == 1) {
                URL toReturn;
                toReturn.type = "boards";
                toReturn.name = "";
                return toReturn;
            } else if (array.size() == 2) {
                URL toReturn;
                toReturn.type = array.front();
                toReturn.name = array.back();
                return toReturn;
            }
            return errorStruct();
        }
        // /board/name         tohto sa to netyka --> /board/name/id --> in getTypeThree
        if (array.front() == "board") {
            if (array.size() == 2) {
                URL toReturn;
                toReturn.type = "board";
                toReturn.name = array.back();
                return toReturn;
            } else if (array.size() == 3) {
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

URL parseUrlPUT(string *rq) {
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

URL parseUrlPOST(string *rq) {
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

URL parseUrlDELETE(string *rq) {
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

URL parseUrlGET(string *rq) {
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

string getUnparsedURL(string *rq) {
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