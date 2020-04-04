CC=g++
CXXFLAGS=-g  #-W -Wall
LDFLAGS= -L./lib/
EXEC=exemple

all: exemple

consoleMenu.so: ./src/*
	$(CC) -o ./lib/consoleMenu.so ./src/*.cpp -shared  $(CXXFLAGS)

$(EXEC): ./exemples/*.cpp
	$(CC) -o ./$(EXEC) ./exemples/*.cpp $(LDFLAGS) -I./src $(CXXFLAGS)

clean: 
	rm ./lib/consoleMenu.so  ./$(EXEC)