//
// Created by olo on 25.10.19.
//

#ifndef ISACPP_DBACCESS_H
#define ISACPP_DBACCESS_H

#include <vector>
#include <string>
#include <map>

int createBoard(std::map<std::string, std::vector<std::string>> &db, const std::string&);
void getBoards(std::map<std::string, std::vector<std::string>> &db, std::string&);
int createPost(std::map<std::string, std::vector<std::string>> &db, const std::string&,const std::string&);

#endif //ISACPP_DBACCESS_H
