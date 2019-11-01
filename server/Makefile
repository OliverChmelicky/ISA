CXX=g++
CXXFLAGS=-std=c++14
LIBOBJ=main.o socket-helper/socketHelper.o argument-parser/parseArgs.o structures/structures.o db-access/dbAccess.o handlers/delete/deleteHandler.o handlers/get/getHandler.o handlers/post/postHandler.o handlers/put/putHandler.o handlers/base-handler/base.o errors/errors.o

all: $(LIBOBJ)
	$(CXX) $(CXXFLAGS) $(LIBOBJ) -o isaserver

clean:
	find . -name "*.o" -type f -delete