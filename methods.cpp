//
// Created by xchmel28 on 19.10.19.
//
#include <string>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include<map>
#include "methods.h"
#include "socketHelper.h"
#include "dbAccess.h"

using namespace std;

void errorNotFound(int);
void errorContentLengthZero(int);
void errorExists(int socket);
body readBody(string, int);


//GET boards --> get all board names
//GET board/"name"  --> get specific board and it`s values
void serveGET(std::string request, int socket, map<string, vector<string>> &db){
    std::string firstLine = request.substr(0, request.find('\n'));
    URL querry = parseUrlGET(&firstLine);

    if (querry.type == "boards"){
        //vrat boards, ak neexistuje ziadna 404
        string allBoards;
        getBoards(db,allBoards);
        std::ostringstream answ;
        answ << "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " <<allBoards.length()<<"\n\n"<< allBoards<<"";
        write(socket, answ.str().data(), answ.str().length() );
        return;
    }
    else if (querry.type == "board"){
        //check ci board/name existuje a vrat
        std::string answ = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nboard";
        write(socket, answ.data(), answ.length() );
        return;
    }

    errorNotFound(socket);
}

// POST boards
// POST board/name
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
            errorContentLengthZero(socket);
            cout << "Length err\n";
            return;
        }

        body newBoard = readBody(request, length);
        if (newBoard.errCode != 0){
            errorNotFound(socket);
            cout << "Body read err\n";
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
            errorContentLengthZero(socket);
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

void servePUT(){
    printf("Implementuj PUT\n");
};

void serveDELETE(){
    printf("Implementuj DELETE\n");
}

void serveUNSUPPORTED(){
    printf("Implementuj UNSUPORTED\n");
    //return errorUnsupportedMethod(socket);
}

void errorContentLengthZero(int socket){
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