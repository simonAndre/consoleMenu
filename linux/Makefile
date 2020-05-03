CC=g++
CXXFLAGS=-g  #-W -Wall
IFLAGS= -I../src
EXEC=exemple
TESTS=./tests/
SOURCES=../src/consoleMenu
CATCHSB=./test/catch_sbox/

all: build run

build: ./*.cpp
	$(CC) ./*.cpp $(SOURCES)/*.cpp  -o ./$(EXEC) $(LDFLAGS) $(IFLAGS) $(CXXFLAGS)  

run: 
	$(EXEC).exe

clean: 
	rm  ./$(EXEC)