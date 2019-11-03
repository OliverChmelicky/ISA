//
// Created by olo on 01.11.19.
//

#ifndef ISA_DELETEHANDLER_H
#define ISA_DELETEHANDLER_H


#include <vector>
#include <map>
#include "../base-handler/base.h"

class deleteHandler: Base {
public:
	static int serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db);
	static URL parseURL(std::string*);
};


#endif //ISA_DELETEHANDLER_H
