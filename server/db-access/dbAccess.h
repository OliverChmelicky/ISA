//
// Created by olo on 25.10.19.
//

#ifndef ISACPP_DBACCESS_H
#define ISACPP_DBACCESS_H

#include <vector>
#include <string>
#include <map>

int createBoard(std::map<std::string, std::vector<std::string>> &db, const std::string& newBoard);
void getBoards(std::map<std::string, std::vector<std::string>> &db, std::string& answer);
int deleteBoard(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName);

int createPost(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName, const std::string& newPost);
int getPosts(std::map<std::string, std::vector<std::string>> &db, const std::string& boardName, std::string& answer);
int updatePost(std::map<std::string, std::vector<std::string>> &db, int index ,const std::string& newPost, const std::string& boardName);
int deletePost(std::map<std::string, std::vector<std::string>> &db, int index , const std::string& boardName);

#endif //ISACPP_DBACCESS_H
