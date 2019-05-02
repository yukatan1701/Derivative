#ifndef __POSTFIX_H__
#define __POSTFIX_H__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "nodes.h"
#include "parser.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

class Postfix {
	friend class Tree;
	bool has_at;
	double point;
	vector<Node *> postfix;
	void buildPostfix(const vector<Node *> & nodes);
public:
	Postfix(const Parser & parser);
	void print() const;
};


#endif
