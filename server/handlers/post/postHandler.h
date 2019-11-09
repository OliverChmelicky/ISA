//
// Created by olo on 01.11.19.
//

#ifndef ISA_POSTHANDLER_H
#define ISA_POSTHANDLER_H


#include <map>
#include <vector>
#include "../../structures/structures.h"
#include "../base-handler/base.h"

class postHandler:Base {
public:
	static int serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db);
	static URL parseURL(std::string*);
};


#endif //ISA_POSTHANDLER_H
