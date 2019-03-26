#ifndef __TREE_H__
#define __TREE_H__

#include "postfix.h"
#include "nodes.h"
#include <vector>
#include <iostream>
#include <queue>
#include <string>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::string;

class Tree {
	Node *root;
	void buildTree(vector<Node *>);
	void print(Node *root) const;
public:
	Tree() {}
	Tree(Node *root): root(root) {}
	Tree(const Postfix & postfix);
	Tree getDerivative() const;
	void print() const;
	string getString() const;
};

#endif
