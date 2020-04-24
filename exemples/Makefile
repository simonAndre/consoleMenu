CC=g++
CXXFLAGS=-g  #-W -Wall
IFLAGS= -I../src
EXEC=exemple
TESTS=./tests/
CATCHSB=./test/catch_sbox/
all: exemple

# $(EXEC): ./*.cpp
# 	$(CC) -o ./$(EXEC) ./*.cpp $(IFLAGS) $(CXXFLAGS)

$(EXEC): ./*.cpp
	$(CC) ../*.cpp ./*.cpp  -o ./$(EXEC) $(LDFLAGS) $(IFLAGS) $(CXXFLAGS)  

run: $(EXEC)
	$(EXEC).exe

all: $(EXEC)
	$(EXEC)
	run
	
clean: 
	rm  ./$(EXEC)