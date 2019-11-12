CXX=g++
CXXFLAGS=-std=c++14 -static-libstdc++
LIBOBJCLIENT=client/main.o client/parser/parser.o client/item-sender/itemSender.o client/board-sender/boardSender.o
LIBOBJSERVER=server/main.o server/socket-helper/socketHelper.o server/argument-parser/parseArgs.o server/structures/structures.o server/db-access/dbAccess.o server/handlers/delete/deleteHandler.o server/handlers/get/getHandler.o server/handlers/post/postHandler.o server/handlers/put/putHandler.o server/handlers/base-handler/base.o server/errors/errors.o

all: $(LIBOBJCLIENT) $(LIBOBJSERVER)
	$(CXX) $(CXXFLAGS) $(LIBOBJCLIENT) -o isaclient
	$(CXX) $(CXXFLAGS) $(LIBOBJSERVER) -o isaserver

client: $(LIBOBJCLIENT)
	$(CXX) $(CXXFLAGS) $(LIBOBJCLIENT) -o isaclient

server: $(LIBOBJSERVER)
	$(CXX) $(CXXFLAGS) $(LIBOBJSERVER) -o isaserver

clean:
	find . -name "*.o" -type f -delete