//
// Created by olo on 28.10.19.
//

#ifndef ISA_PARSER_H
#define ISA_PARSER_H

#include <string>
#include "../enums/enums.h"

using namespace std;

struct arguments {
    string host;
    int port;
    type commandType;
    operation commandOperation;
    string boardName;
    string content;
    int id;
    int errorCode;
};

class parser {
public:
    static arguments parse(int argc, char *argv[]);
    static int stringToInt(string convert);
    static bool regexOnBoard(const string& check);
};


#endif //ISA_PARSER_H
