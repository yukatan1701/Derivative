#include "tree.h"

void Tree::buildTree(const vector<Node *> nodes)
{
	stack<Node *> stack;
	for (Node *node: nodes) {
		if (node->getNodeType() == TERM) {
			stack.push(node);
		} else {
			Operator *op = static_cast<Operator *>(node);
			Node *right_child = stack.top();
			stack.pop();
			op->setRightChild(right_child);
			if (op->isBinary()) {
				Node *left_child = stack.top();
				stack.pop();
				op->setLeftChild(left_child);
			}
			stack.push(op);
		}
	}
	root = stack.top();
}

Tree::Tree(const Postfix & postfix)
{
	vector<Node *> nodes = postfix.postfix;
	buildTree(nodes);
}

Tree Tree::getDerivative() const
{
	Node *new_root = root->differentiate();
	return Tree(new_root);
}

void Tree::print(Node *root) const
{
	if (root == nullptr)
		return;
	if (root->getNodeType() == OPERATOR) {
		Operator *op = static_cast<Operator *>(root);
		cout << "(";
		print(op->getLeftChild());
	}
	root->print();
	if (root->getNodeType() == OPERATOR) {
		Operator *op = static_cast<Operator *>(root);
		print(op->getRightChild());
		cout << ")";
	}
}

void Tree::print() const
{
	print(root);
	cout << endl;
}

string Tree::getString() const
{
	string str;
	return str;
}