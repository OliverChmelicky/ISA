//
// Created by xchmel28 on 25.10.19.
//
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "dbAccess.h"

using namespace std;

int createBoard(std::map<std::string, std::vector<std::string>> &db, const string& boardName){
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it != db.end()) //Board name exist
        return -1;
    db[boardName] = vector<string>();
    return 0;
}

void getBoards(std::map<std::string, std::vector<std::string>> &db, string& answer){
    for(auto elem : db)
    {
        answer = answer.append(elem.first + "\n");
    }
}

int createPost(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName,const string& newPost) {
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;
    db[boardName].push_back(newPost);
    return 0;
}
