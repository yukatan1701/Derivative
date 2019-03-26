all: hello

hello:
	g++ main.cpp nodes.cpp tree.cpp parser.cpp postfix.cpp -o main -Wall -std=c++11 -fmax-errors=5
	
