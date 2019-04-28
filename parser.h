#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "nodes.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Parser {
	friend class Postfix;
	vector<Node *> nodes;
	bool has_at;
	double point;
	Number *readNumber(const string & input, unsigned int & i);
	Operator *getOperatorByName(string name);
	Node *wordToNode(string input, unsigned int & i);
	bool isSign(char ch);
public:
	Parser(string formula);
	~Parser();
	void print() const;
};

#endif
