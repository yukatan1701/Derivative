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

const double EPS = 0.00001;

class Tree {
	Node *root;
	void buildTree(vector<Node *>);
	void simplify();
	void print(Node *root) const;
	void deleteBranch(Node *cur_root) const;
	Node *processLeftUseless(Binary *bin) const;
	Node *processRightUseless(Binary *bin) const;
	Node *processUseless(Node *cur_root) const;
	Node *doArithmetic(Binary *bin) const;
	Node *processArithmetic(Node *cur_root) const;
	double value(Node *cur_root, double point) const;
	double dvalue(Node *cur_root, double point) const;
public:
	Tree() {}
	Tree(Node *root): root(root) {}
	Tree(const Postfix & postfix);
	Tree getDerivative() const;
	void print() const;
	string getString() const;
	double value(double point) const;
	double dvalue(double point) const;
};

#endif
