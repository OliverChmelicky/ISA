//
// Created by olo on 19.10.19.
//

#ifndef ISA_METHODS_H
#define ISA_METHODS_H
#include<bits/stdc++.h>

struct body
{
    std::string content;
    int errCode;
};

struct dbRead{
    std::string content;
    int id;
    int errCode;
};

void serveGET(std::string, int, std::map<std::string, std::vector<std::string>> &db);
void servePOST(std::string, int, std::map<std::string, std::vector<std::string>> &db);
void servePUT();
void serveDELETE();
void serveUNSUPPORTED();

#endif //ISA_METHODS_H
