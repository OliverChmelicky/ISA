//
// Created by olo on 31.10.19.
//

#ifndef ISA_STRUCTURES_H
#define ISA_STRUCTURES_H

#include <string>

struct URL {
	std::string type;
	std::string name;
	int id;
};

struct body
{
	std::string content;
	int errCode;
};

URL errorStruct();

#endif //ISA_STRUCTURES_H
