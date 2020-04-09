CC=g++
CXXFLAGS=-g  #-W -Wall
IFLAGS= -I../src
EXEC=exemple
TESTS=./tests/
CATCHSB=./test/catch_sbox/
all: exemple

$(EXEC): ./*.cpp
	$(CC) -o ./$(EXEC) ./*.cpp $(IFLAGS) $(CXXFLAGS)

all: $(EXEC)

clean: 
	rm  ./$(EXEC)