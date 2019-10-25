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
void errorBoardExists(int socket);
body readBody(string, int);


//GET boards --> get all board names
//GET board/"name"  --> get specific board and it`s values
void serveGET(std::string request, int socket, map<string, vector<string>> &db){
    std::string firstLine = request.substr(0, request.find('\n'));
    URL querry = getParsedURL(&firstLine);

    //one or two names in querry
    if (querry.type == "boards"){
        //vrat boards, ak neexistuje ziadna 404
        //ak mam name alebo id v URL vrat 404
        std::string answ = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 14\n\nboards";
        write(socket, answ.data(), answ.length() );
        return;
    }
    // two or three names in querry
    else if (querry.type == "board"){
        //check ci board/name existuje a vrat
        //check ze nesmie mat ID a vrat 404
        std::string answ = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nboard";
        write(socket, answ.data(), answ.length() );
        return;
    }

    errorNotFound(socket);
}

//
//
void servePOST(std::string request, int socket, map<string, vector<string>> &db) {
    printf("Implementuj POST\n");
    std::string firstLine = request.substr(0, request.find('\n'));
    URL query = getParsedURL(&firstLine);
    int length;

    if (query.type == "boards"){
        if (query.name.empty()){
            errorNotFound(socket);
            cout << "Not BOARDS err";
            return;
        }
        if (!contentTypeTxt(request)){
            errorNotFound(socket);
            cout << "Content type err";
            return;
        }
        length = contentLength(request);
        if (length <= 0){
            errorContentLengthZero(socket);
            cout << "Length err";
            return;
        }

        //reading from body
        body newBoard = readBody(request, length);
        if (newBoard.errCode != 0){
            errorNotFound(socket);
            cout << "Body read err";
            return;
        }
        if ( 0 != createBoard(db, newBoard.content) ){
            errorBoardExists(socket);
            cout<< "Boar exists err";
            return;
        }

        //uloz
    }
    else if (query.type == "board") {
        if (query.name.empty()){
            errorNotFound(socket);
            return;
        }
    }

    std::string answ = "HTTP/1.1 404 PRESLO KAMO\nContent-Type: text/plain\nContent-Length: 14\n\nPOST";
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

void errorBoardExists(int socket){
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