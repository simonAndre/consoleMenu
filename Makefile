CC=g++
CXXFLAGS=-g  #-W -Wall
LDFLAGS= -L./lib/
IFLAGS= -I./src
EXEC=exemple
TESTS=./tests/
CATCHSB=./test/catch_sbox/
all: exemple

consoleMenu.so: ./src/*
	$(CC) -o ./lib/consoleMenu.so ./src/*.cpp -shared  $(CXXFLAGS)

$(EXEC): ./exemples/*.cpp
	$(CC) -o ./$(EXEC) ./exemples/*.cpp $(LDFLAGS) $(IFLAGS) $(CXXFLAGS) && ./exemple.exe

tests-prep:	$(TESTS)tests-main.cpp
	$(CC) -o $(TESTS)tests-main.o $(TESTS)tests-main.cpp -c

# tests-target:	$(TESTS)consolemenu.cpp
# 	$(CC) -o $(TESTS)consolemenu.o $(TESTS)consolemenu.cpp -c $(IFLAGS)
	
tests: $(TESTS)tests-consolemenu.cpp
	$(CC) $(TESTS)tests-main.o $(TESTS)tests-consolemenu.cpp -o $(TESTS)tests $(IFLAGS) && $(TESTS)tests.exe

catchsboxprep:	$(CATCHSB)tests-main.cpp
	$(CC) -o $(CATCHSB)tests-main.o $(CATCHSB)tests-main.cpp -c

catchsbox-target:	$(CATCHSB)factorial.cpp
	$(CC) -o $(CATCHSB)factorial.o $(CATCHSB)factorial.cpp -c

catchsbox:	catchsbox-target catchsboxprep $(CATCHSB)tests-factorial.cpp
	$(CC) $(CATCHSB)tests-main.o $(CATCHSB)factorial.o $(CATCHSB)tests-factorial.cpp -o $(CATCHSB)tests && $(CATCHSB)tests.exe


clean: 
	rm ./lib/consoleMenu.so  ./$(EXEC)