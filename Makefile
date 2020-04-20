CC=g++
CXXFLAGS=-g -W -Wall -pedantic
LIBDIR=./lib/
SRCDIR=./
LDFLAGS= -L$(LIBDIR)
IFLAGS= -I./src -I./src/consoleMenu -I./src/consoleMenu/IO -I./src/consoleMenu/Menu
EXEC=exemple
TESTS=./tests/
CATCHSB=./test/catch_sbox/
all: exemple

io: $(SRCDIR)consoleMenu/IO/*.hpp
	$(CC) -o $(LIBDIR)io.so $(SRCDIR)consoleMenu/IO/*.hpp -shared  $(CXXFLAGS)

cmenu: $(SRCDIR)*
	$(CC) -o $(LIBDIR)consoleMenu.so $(SRCDIR)*.cpp  -shared  $(CXXFLAGS) $(IFLAGS)


$(EXEC): ./exemples/*.cpp
	$(CC) $(SRCDIR)*.cpp ./exemples/*.cpp  -o ./$(EXEC) $(LDFLAGS) $(IFLAGS) $(CXXFLAGS)
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


# cmenu_nolinker: $(SRCDIR)*
# 	$(CC) -Wall -pedantic -c $(SRCDIR)*.cpp  $(CXXFLAGS) $(IFLAGS)
# exemple_nolinker: ./exemples/*.cpp
# 	$(CC) -Wall -pedantic -c ./exemples/*.cpp  $(CXXFLAGS) $(IFLAGS)
# exemple_link:
# 	$(CC) -Wall -pedantic ./*.o -o exemple.exe

