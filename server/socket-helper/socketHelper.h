//
// Created by olo on 19.10.19.
//
#include <string>
#include "../enums/enums.h"
#include "../structures/structures.h"

#ifndef ISA_SOCKETHELPER_H
#define ISA_SOCKETHELPER_H

int serveUnsupported(int socket);
requestMethod getMethod(const std::string&);
std::string removeSpaces(std::string);
int convertToInt(std::string);
bool contentTypeTxt(std::string);
int contentLength(std::string);
std::string trim(const std::string&);
int checkProtocol(const std::string& request);



#endif //ISA_SOCKETHELPER_H
