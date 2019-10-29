cmake_minimum_required(VERSION 3.15)
project(ISA)

set(CMAKE_CXX_STANDARD 14)

add_executable(ISACpp server/main.cpp server/methods.cpp server/socketHelper.cpp server/parseArgs.cpp server/dbAccess.cpp client/main.cpp client/parser/parser.cpp client/parser/parser.h client/enums/enums.h client/board-sender/boardSender.cpp client/board-sender/boardSender.h client/item-sender/itemSender.cpp client/item-sender/itemSender.h)