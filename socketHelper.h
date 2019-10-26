//
// Created by olo on 19.10.19.
//
#include <string>
#include "enums.h"

#ifndef ISA_SOCKETHELPER_H
#define ISA_SOCKETHELPER_H

struct URL {
    std::string type;
    std::string name;
    int id;
};

requestMethod getMethod(std::string);
std::string getUnparsedURL(std::string*);
//URL getParsedURL(std::string*);
URL parseUrlGET(std::string*);
URL parseUrlPOST(std::string*);
std::string removeSpaces(std::string);
int convertToInt(std::string);
bool contentTypeTxt(std::string);
int contentLength(std::string);
std::string trim(const std::string&);



#endif //ISA_SOCKETHELPER_H
