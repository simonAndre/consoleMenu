CC=g++
CXXFLAGS=-g -W -Wall -pedantic
LIBDIR=./lib/
LDFLAGS= -L$(LIBDIR)
IFLAGS= -I./src -I./src/consoleMenu -I./src/consoleMenu/IO -I./src/consoleMenu/Menu
EXEC=exemple
TESTS=./tests/
CATCHSB=./test/catch_sbox/
all: exemple

io: ./src/consoleMenu/IO/*.hpp
	$(CC) -o $(LIBDIR)io.so ./src/consoleMenu/IO/*.hpp -shared  $(CXXFLAGS)

cmenu: ./src/*
	$(CC) -o $(LIBDIR)consoleMenu.so ./src/consoleMenu/Menu/*.cpp  -shared  $(CXXFLAGS) $(IFLAGS)

check: ./src/*
	$(CC) -o $(LIBDIR)check.so ./src/check.cpp -shared  $(CXXFLAGS) $(IFLAGS)


$(EXEC): ./exemples/*.cpp
	$(CC) ./src/consoleMenu/Menu/*.cpp ./exemples/*.cpp  -o ./$(EXEC) $(LDFLAGS) $(IFLAGS) $(CXXFLAGS)  && ./exemple.exe
	# $(CC) ./exemples/*.cpp -l consoleMenu.so -o ./$(EXEC) $(LDFLAGS) $(IFLAGS) $(CXXFLAGS)  

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
	rm $(LIBDIR)consoleMenu.so 
	rm $(LIBDIR)io.so
	rm ./$(EXEC)


# cmenu_nolinker: ./src/*
# 	$(CC) -Wall -pedantic -c ./src/consoleMenu/Menu/*.cpp  $(CXXFLAGS) $(IFLAGS)
# exemple_nolinker: ./exemples/*.cpp
# 	$(CC) -Wall -pedantic -c ./exemples/*.cpp  $(CXXFLAGS) $(IFLAGS)
# exemple_link:
# 	$(CC) -Wall -pedantic ./*.o -o exemple.exe

