consolemenu: ./src/*
	g++ -pg ./src/*.cpp -shared -o ./lib/consoleMenu.so
test: ./tests/*.cpp
	g++ -pg ./tests/*.cpp -L./lib -l:consoleMenu.so -I./src -o ./test.exe
	cp ./test.exe ./tests/test_menu.exe