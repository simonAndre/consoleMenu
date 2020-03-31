CC=g++
CXXFLAGS=-g  #-W -Wall
LDFLAGS= -L./lib/
EXEC=exemple

all: exemple

consoleMenu.so: ./src/*
	$(CC) -o ./lib/consoleMenu.so ./src/*.cpp -shared  $(CXXFLAGS)

$(EXEC): ./exemples/*.cpp ./src/*.cpp
	$(CC) -o ./$(EXEC) ./src/*.cpp ./exemples/*.cpp $(LDFLAGS) -I./src $(CXXFLAGS)

clean: 
	rm ./lib/consoleMenu.so  ./$(EXEC)