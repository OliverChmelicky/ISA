CXX=g++
CXXFLAGS=-std=c++14
LIBOBJ=main.o parser/parser.o item-sender/itemSender.o board-sender/boardSender.o

all: $(LIBOBJ)
	$(CXX) $(CXXFLAGS) $(LIBOBJ) -o isaclient

clean:
	find . -name "*.o" -type f -delete