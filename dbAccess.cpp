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

int deleteBoard(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName){
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;
    db.erase(it);
    return 0;
}



int createPost(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName,const string& newPost) {
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;
    db[boardName].push_back(newPost);
    return 0;
}

int getPosts(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName,string& answer){
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;

    answer = answer.append("[" + boardName + "]\n");

    for(int i = 0; i < db[boardName].size(); i++)
    {
        answer = answer.append( to_string(i+1) +  ". ");
        answer = answer.append(db[boardName][i] + ".\n");
    }

    return 0;
}

int updatePost(std::map<std::string, std::vector<std::string>> &db, int index ,const std::string& newPost, const std::string& boardName){
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;
    if( db[boardName].size() < index) //Board has less posts than we want to delete
        return -1;

    db[boardName][index-1] = newPost;

    cout<< "Novy ost je";
    cout<< newPost;

    return 0;
}

int deletePost(std::map<std::string, std::vector<std::string>> &db, int index , const std::string& boardName){
    map<std::string, std::vector<std::string>>::iterator it ;
    it = db.find(boardName);

    if(it == db.end()) //Board name does NOT exist
        return -1;
    if( db[boardName].size() < index) //Board has less posts than we want to delete
        return -1;

    db[boardName].erase(db[boardName].begin()+index-1);
    return 0;
}