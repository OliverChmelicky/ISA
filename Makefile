CXX=g++
CXXFLAGS=-std=c++14
LIBOBJ=main.o methods.o socketHelper.o parseArgs.o dbAccess.o

all: $(LIBOBJ)
	$(CXX) $(CXXFLAGS) $(LIBOBJ) -o server
