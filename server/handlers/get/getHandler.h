//
// Created by olo on 31.10.19.
//

#ifndef ISA_GETHANDLER_H
#define ISA_GETHANDLER_H
#include<bits/stdc++.h>

#include "../../structures/structures.h"
#include "../base-handler/base.h"

class getHandler: Base {
public:
	static int serve(std::string, int, std::map<std::string, std::vector<std::string>> &db);
	static URL parseURL(std::string*);
};


#endif //ISA_GETHANDLER_H
