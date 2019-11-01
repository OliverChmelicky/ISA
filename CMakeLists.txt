cmake_minimum_required(VERSION 3.15)
project(ISA)

set(CMAKE_CXX_STANDARD 14)

add_executable(ISACpp server/main.cpp server/socket-helper/socketHelper.cpp server/argument-parser/parseArgs.cpp server/db-access/dbAccess.cpp client/main.cpp client/parser/parser.cpp client/parser/parser.h client/enums/enums.h client/board-sender/boardSender.cpp client/board-sender/boardSender.h client/item-sender/itemSender.cpp client/item-sender/itemSender.h server/handlers/get/getHandler.cpp server/handlers/get/getHandler.h server/errors/errors.cpp server/errors/errors.h server/structures/structures.h server/handlers/base-handler/base.cpp server/handlers/base-handler/base.h server/handlers/post/postHandler.cpp server/handlers/post/postHandler.h server/handlers/put/putHandler.cpp server/handlers/put/putHandler.h server/handlers/delete/deleteHandler.cpp server/handlers/delete/deleteHandler.h server/structures/structures.cpp)