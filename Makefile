SRCDIR=sources
INCDIR=include
FILES=nodes.cpp tree.cpp parser.cpp postfix.cpp
SOURCES=$(FILES:%.cpp=$(SRCDIR)/%.cpp)

all: hello

hello:
	g++ main.cpp $(SOURCES) -I $(INCDIR) -o main -Wall -std=c++11 -fmax-errors=5
	
