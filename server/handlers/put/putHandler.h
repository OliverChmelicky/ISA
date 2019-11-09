//
// Created by olo on 01.11.19.
//

#ifndef ISA_PUTHANDLER_H
#define ISA_PUTHANDLER_H


#include "../base-handler/base.h"

class putHandler: Base {
public:
	static int serve(const std::string& request, int socket, std::map<std::string, std::vector<std::string>> &db);
	static URL parseUrl(std::string *rq);
};


#endif //ISA_PUTHANDLER_H
