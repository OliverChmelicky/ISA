//
// Created by xchmel28 on 19.10.19.
//
#include <string>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include<map>
#include <iostream>
#include <string>
#include <regex>
#include <iomanip>
#include "methods.h"
#include "socketHelper.h"
#include "dbAccess.h"

using namespace std;

void errorNotFound(int);
void errorBadRequest(int);
void errorExists(int socket);
body readBody(string, int);


//GET boards --> get all board names
//GET board/[name]  --> get specific board and it`s values
void serveGET(std::string request, int socket, map<string, vector<string>> &db){
    std::string firstLine = request.substr(0, request.find('\n'));
    URL querry = parseUrlGET(&firstLine);

    if (querry.type == "boards"){
        string allBoards;
        getBoards(db,allBoards);
        std::ostringstream answ;
        answ << "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " <<allBoards.length()<<"\n\n"<< allBoards<<"";
        write(socket, answ.str().data(), answ.str().length() );
        return;
    }
    else if (querry.type == "board"){
        //check ci board/name existuje a vrat
        string posts;
        if (0 != getPosts(db, querry.name,posts)){
            errorNotFound(socket);
            cout << "Board was not found\n";
            return;
        }
        std::ostringstream answ;
        answ << "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " <<posts.length()<<"\n\n"<< posts<<"";
        write(socket, answ.str().data(), answ.str().length() );
        return;
    }

    errorNotFound(socket);
}

// POST boards
// POST board/[name]
void servePOST(std::string request, int socket, map<string, vector<string>> &db) {
    std::string answ = "HTTP/1.1 500 Internal Server Error\nContent-Length: 0\n\n";
    std::string firstLine = request.substr(0, request.find('\n'));
    URL query = parseUrlPOST(&firstLine);
    int length;

    if (query.type == "boards"){
        if (query.name.empty()){
            errorNotFound(socket);
            cout << "Not BOARDS err\n";
            return;
        }
        if (!contentTypeTxt(request)){
            errorNotFound(socket);
            cout << "Content type err\n";
            return;
        }
        length = contentLength(request);
        if (length <= 0){
            errorBadRequest(socket);
            cout << "Length err\n";
            return;
        }

        body newBoard = readBody(request, length);
        if (newBoard.errCode != 0){
            errorBadRequest(socket);
            cout << "Body read err\n";
            return;
        }
        if (query.name != newBoard.content){
            errorBadRequest(socket);
            cout << "Body does not match URL\n";
            return;
        }
        const std::regex re( "[a-zA-Z0-9]*" ) ;
        std::smatch match ;
        if( !std::regex_match( query.name, match, re ) ){
            errorBadRequest(socket);
            cout << "Regex err\n";
            return;
        }
        if ( 0 != createBoard(db, newBoard.content) ){
            errorExists(socket);
            cout<< "Board exists err\n";
            return;
        }

        answ = "HTTP/1.1 201 Created\nContent-Length: 0\n\n";
    }
    else if (query.type == "board") {
        if (query.name.empty()){
            errorNotFound(socket);
            return;
        }
        if (!contentTypeTxt(request)){
            errorNotFound(socket);
            cout << "Content type err\n";
            return;
        }
        length = contentLength(request);
        if (length <= 0){
            errorBadRequest(socket);
            cout << "Length err\n";
            return;
        }

        body newPost = readBody(request, length);
        if (newPost.errCode != 0){
            errorNotFound(socket);
            cout << "Body read err\n";
            return;
        }
        if ( 0 != createPost(db, query.name,newPost.content) ){
            errorNotFound(socket);
            cout<< "Board does not exist\n";
            return;
        }
        answ = "HTTP/1.1 201 Created\nContent-Length: 0\n\n";
    }
    write(socket, answ.data(), answ.length() );
}

//  /board/[name]/[index]
void servePUT(std::string request, int socket, map<string, vector<string>> &db){
    std::string answ = "HTTP/1.1 500 Internal Server Error\nContent-Length: 0\n\n";
    std::string firstLine = request.substr(0, request.find('\n'));
    URL query = parseUrlPUT(&firstLine);
    int length;

    if (query.type == "board") {
        if (query.name.empty()){
            errorNotFound(socket);
            return;
        }
        if (!contentTypeTxt(request)){
            errorNotFound(socket);
            cout << "Content type err\n";
            return;
        }
        length = contentLength(request);
        if (length <= 0){
            errorBadRequest(socket);
            cout << "Length err\n";
            return;
        }

        body newPost = readBody(request, length);
        if (newPost.errCode != 0){
            errorNotFound(socket);
            cout << "Body read err\n";
            return;
        }

        if ( 0 != updatePost(db, query.id, newPost.content,query.name) ){
            errorNotFound(socket);
            cout<< "Board does not exist\n";
            return;
        }
        answ = "HTTP/1.1 200 OK\nContent-Length: 0\n\n";
    }

    write(socket, answ.data(), answ.length() );
};

void serveDELETE(std::string request, int socket, map<string, vector<string>> &db){
    std::string answ = "HTTP/1.1 500 Internal Server Error\nContent-Length: 0\n\n";
    std::string firstLine = request.substr(0, request.find('\n'));
    URL query = parseUrlDELETE(&firstLine);

    if (query.type == "boards"){
        if (query.name.empty()){
            errorNotFound(socket);
            cout << "Not BOARDS err\n";
            return;
        }
        if ( 0 != deleteBoard(db, query.name) ){
            errorNotFound(socket);
            cout<< "Board does not exist\n";
            return;
        }
        answ = "HTTP/1.1 200 OK\nContent-Length: 0\n\n";
    }
    if (query.type == "board") {
        if (query.name.empty()){
            errorNotFound(socket);
            return;
        }
        if ( 0 != deletePost(db, query.id, query.name) ){
            errorNotFound(socket);
            cout<< "Board does not exist or id is not in range\n";
            return;
        }
        answ = "HTTP/1.1 200 OK\nContent-Length: 0\n\n";
    }

    cout<<"ID :";
    cout<<query.id<< endl;
    cout<<"NAME :";
    cout<<query.name<<endl;
    cout<<"TYPE :";
    cout<<query.type<<endl;

    write(socket, answ.data(), answ.length() );
};

void serveUNSUPPORTED(int socket){
    std::string answ = "HTTP/1.1 405 Method Not Allowed\nContent-Length: 0\n\n";
    write(socket, answ.data(), answ.length() );
}

void errorBadRequest(int socket){
    std::string answ = "HTTP/1.1 400 Bad Request\nContent-Length: 0\n\n";
    write(socket, answ.data(), answ.length() );
}

void errorNotFound(int socket){
    std::string answ = "HTTP/1.1 404 Not Found\nContent-Length: 0\n\n";
    write(socket, answ.data(), answ.length() );
}

void errorExists(int socket){
    std::string answ = "HTTP/1.1 409 Conflict\nContent-Length: 0\n\n";
    write(socket, answ.data(), answ.length() );
}

body readBody(string request, int length){
    size_t pos = 0;
    std::string bodyString;
    string delimiter = "\r\n\r\n";
    struct body bodyToReturn;

    pos = request.find(delimiter);
    pos += 4;
    if (pos > request.size()){
        bodyToReturn.errCode = 1;
        return bodyToReturn;
    }
    bodyString = request.substr(pos, request.size());

    if (bodyString.length() < length)
        length = bodyString.length();
    bodyString = trim(bodyString.substr(0, length));

    bodyToReturn.errCode = 0;
    bodyToReturn.content = bodyString;

    if (bodyString.empty())
        bodyToReturn.errCode = 1;

    return bodyToReturn;
}


//Vypis ascii znakov zo str
//for (int i = 0; i < str.size(); i++){
//        cout<< int(str[i])<<endl;
//    }