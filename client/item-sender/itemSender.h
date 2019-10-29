//
// Created by olo on 28.10.19.
//

#ifndef ISA_ITEMSENDER_H
#define ISA_ITEMSENDER_H


#include "../parser/parser.h"

class itemSender {
public:
    static int sendItemRequest(int socket, const arguments& data);
};


#endif //ISA_ITEMSENDER_H
