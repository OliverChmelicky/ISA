//
// Created by olo on 31.10.19.
//

#ifndef ISA_BASE_H
#define ISA_BASE_H


#include <string>
#include "../../structures/structures.h"

class Base {
public:
	static std::string getUnparsedURL(std::string *rq);
	static body readBody(std::string request, int length);
};


#endif //ISA_BASE_H
