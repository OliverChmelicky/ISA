//
// Created by xchmel28 on 28.10.19.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <regex>
#include "parser.h"
#include "../enums/enums.h"

arguments parser::parse(int argc, char **argv) {
    arguments retval;
    retval.errorCode = 0;

	if (argc == 2 && (0 == strcmp(argv[1], "-h")) ){
		std::cout << "Usage:  ./isaclient -H <host> -p <port> <command>\n";
		retval.errorCode = 6378;
		return retval;
	}
    if (argc < 6 || argc > 10) {
        retval.errorCode = -1;
        return retval;
    }


    for (int i = 1; i < argc; ++i) { // Remember argv[0] is the path to the program, we want from argv[1] onwards
        if ( i == 1 && 0 != strcmp(argv[i], "-H")){
            retval.errorCode = -1;
            return retval;
        }
        if ( i == 2){
            retval.host = argv[i];
            continue;
        }
        if (i == 3 && 0 != strcmp(argv[i], "-p")){
            retval.errorCode = -1;
            break;
        }
        if (i == 4){
            int port = parser::stringToInt(argv[i]) ;
            if(port < 0){
                retval.errorCode = -1;
                break;
            }
            retval.port = port;
        }

        //od 5 zacinaju commandy
        //boards + 0
        //board + 2 posledny argument len alfanumericke znaky
        //item + 3 posledny argument je premennej velkosti
        if (i == 5){
            if ( 0 == strcmp(argv[i],"boards") && argc == 6){
                retval.commandType = BOARDS;
                return retval;
            }
            else if ( 0 == strcmp(argv[i],"board") && argc == 8){
                if ( 0 == strcmp(argv[i+1], "add") ){
                    retval.commandType = BOARD;
                    retval.commandOperation = ADD;
                    retval.boardName = argv[i+2];
                }
                else if (0 == strcmp(argv[i+1], "delete")){
                    retval.commandType = BOARD;
                    retval.commandOperation = DELETE;
                    retval.boardName = argv[i+2];
                }
                else if (0 == strcmp(argv[i+1], "list")){
                    retval.commandType = BOARD;
                    retval.commandOperation = LIST;
                    retval.boardName = argv[i+2];
                }
                else{
                    retval.errorCode = -1;
                }
                if (!parser::regexOnBoard(retval.boardName)){
                    retval.errorCode = -1;
                }
                return retval;
            }
            else if ( 0 == strcmp(argv[i],"item") && argc == 9){
                if ( 0 == strcmp(argv[i+1], "add") ){
                    retval.commandType = ITEM;
                    retval.commandOperation = ADD;
                    retval.boardName = argv[i+2];
                    retval.content = argv[i+3];
                }
                else if (0 == strcmp(argv[i+1], "delete")){
                    retval.commandType = ITEM;
                    retval.commandOperation = DELETE;
                    retval.boardName = argv[i+2];
                    int idInt = parser::stringToInt(argv[i+3]);
                    if ( idInt < 0 ){
                        retval.errorCode = -1;
                        return retval;
                    }
                    retval.id = idInt;
                }
                else{
                    retval.errorCode = -1;
                    return retval;
                }

                if (!parser::regexOnBoard(retval.boardName)){
                    retval.errorCode = -1;
                }
                return retval;

            }
            else if ( 0 == strcmp(argv[i],"item") && argc == 10){
				if (0 == strcmp(argv[i+1], "update")){
					retval.commandType = ITEM;
					retval.commandOperation = UPDATE;
					retval.boardName = argv[i+2];
					retval.content = argv[i+4];
					int idInt = parser::stringToInt(argv[i+3]);
					if ( idInt < 0 ){
						retval.errorCode = -1;
						return retval;
					}
					retval.id = idInt;
				}
				else{
					retval.errorCode = -1;
					return retval;
				}
				return retval;
            }
            else {
                retval.errorCode = -1;
                return retval;
            }
        }
    }

    cout << "Parser came to an end"<<endl;
    retval.errorCode = -1;
    return retval;


}

int parser::stringToInt(string convert) {
        int i = 0;
        for (char c: convert) {
            if (c >= '0' && c <= '9') {
                i = i * 10 + (c - '0');
            } else {
                return -1;
            }
        }
        return std::atoi(convert.c_str());
}

bool parser::regexOnBoard(const string &check) {
    const std::regex re( "[a-zA-Z0-9]*" ) ;
    std::smatch match ;
    bool result;
    result = std::regex_match(check, match, re);
    if (!result){
    	cout<<"Error regex in boarname\n";
		return false;
    }
	return true;
};
