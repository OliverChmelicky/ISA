//
// Created by xchmel28 on 28.10.19.
//

#ifndef ISA_BOARDSENDER_H
#define ISA_BOARDSENDER_H


#include "../parser/parser.h"

class boardSender {
public:
    static int sendBoardRequest(int socket, const arguments& data);
};


#endif //ISA_BOARDSENDER_H
