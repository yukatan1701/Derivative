#include "tree.h"

void Tree::buildTree(const vector<Node *> nodes)
{
	stack<Node *> stack;
	for (Node *node: nodes) {
		if (node->getNodeType() == TERM) {
			stack.push(node);
		} else {
			Operator *op = static_cast<Operator *>(node);
			if (op->isBinary()) {
				Binary *bin = static_cast<Binary *>(op);
				Node *right_child = stack.top();
				stack.pop();
				Node *left_child = stack.top();
				stack.pop();
				bin->setChilds(left_child, right_child);
			} else {
				Function *func = static_cast<Function *>(op);
				func->setArg(stack.top());
				stack.pop();
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
	Node *new_root = root->diff();
	Tree tree(new_root);
	tree.print();
	tree.simplify();
	return tree;
}

void Tree::print(Node *root) const
{
	if (root == nullptr)
		return;
	if (root->getNodeType() == OPERATOR) {
		Operator *op = static_cast<Operator *>(root);
		cout << "(";
		if (op->isBinary()) {
			Binary *bin = static_cast<Binary *>(op);
			print(bin->getLeftChild());
		}
	}
	root->print();
	if (root->getNodeType() == OPERATOR) {
		Operator *op = static_cast<Operator *>(root);
		if (op->isBinary()) {
			Binary *bin = static_cast<Binary *>(op);
			print(bin->getRightChild());
		} else {
			Function *func = static_cast<Function *>(op);
			print(func->getArg());
		}
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

void Tree::deleteBranch(Node *cur_root) const
{
	if (cur_root == nullptr)
		return;
	if (cur_root->getNodeType() == OPERATOR) {
		Operator *op = static_cast<Operator *>(cur_root);
		if (op->isBinary()) {
			Binary *bin = static_cast<Binary *>(op);
			deleteBranch(bin->getLeftChild());
			deleteBranch(bin->getRightChild());
		} else {
			Function *func = static_cast<Function *>(op);
			deleteBranch(func->getArg());
		}
		delete op;
	} else {
		Term *term = static_cast<Term *>(cur_root);
		delete term;
	}
}

Node * Tree::processLeftUseless(Binary *bin) const
{
	Node *left = bin->getLeftChild(), *right = bin->getRightChild();
	Term *term = static_cast<Term *>(left);
	if (term->getTermType() == NUMBER) {
		Number *num = static_cast<Number *>(term);
		double val = num->getValue();
		if (abs(val - 0.0) < EPS && (bin->getType() == MULT ||
			bin->getType() == DIV)) {
			deleteBranch(right);
			delete bin;
			return num;
		} else if ((abs(val - 1.0) < EPS && bin->getType() == MULT) ||
			(abs(val - 0.0) < EPS && bin->getType() == PLUS)) {
			delete bin;
			delete num;
			return processUseless(right);
		}
	}
	return bin;
}

Node * Tree::processRightUseless(Binary *bin) const
{
	Node *left = bin->getLeftChild(), *right = bin->getRightChild();
	Term *term = static_cast<Term *>(right);
	if (term->getTermType() == NUMBER) {
		Number *num = static_cast<Number *>(term);
		double val = num->getValue();
		if (abs(val - 0.0) < EPS && bin->getType() == MULT) {
			deleteBranch(left);
			delete bin;
			return num;
		} else if ((abs(val - 1) < EPS && bin->getType() == MULT) ||
			(abs(val - 0.0) < EPS && bin->getType() == PLUS)) {
			delete bin;
			delete num;
			return processUseless(left);
		}
	}
	return bin;
}

Node * Tree::processUseless(Node *cur_root) const
{
	if (cur_root == nullptr || cur_root->getNodeType() == TERM)
		return cur_root;
	Operator *op = static_cast<Operator *>(cur_root);
	if (op->isBinary()) {
		Binary *bin = static_cast<Binary *>(op);
		Node *left = bin->getLeftChild(), *right = bin->getRightChild();
		NODE_TYPE ltype = left->getNodeType(), rtype = right->getNodeType();
		if (ltype == OPERATOR && rtype == OPERATOR) {
			bin->setChilds(processUseless(left), processUseless(right));
			return bin;
		}
		if (ltype == TERM) {		
			Term *term = static_cast<Term *>(left);
			if (term->getTermType() == NUMBER)
				return processLeftUseless(bin);
			return processRightUseless(bin);
		} else {
			Term *term = static_cast<Term *>(right);
			if (term->getTermType() == NUMBER)
				return processRightUseless(bin);
			return processLeftUseless(bin);	
		}
	} else { 
		Function *func = static_cast<Function *>(op);
		if (func->getArg()->getNodeType() != TERM) {
			func->setArg(processUseless(func->getArg()));
			return func;
		}
	}
	return cur_root;
}

Node * Tree::doArithmetic(Binary *bin) const
{	
	Term *left = static_cast<Term *>(bin->getLeftChild());
	Term *right = static_cast<Term *>(bin->getRightChild());
	TERM_TYPE ltype = left->getTermType(), rtype = right->getTermType();
	if (ltype == NUMBER && rtype == NUMBER) {
		double lval = static_cast<Number *>(left)->getValue();
		double rval = static_cast<Number *>(right)->getValue();
		double ans = bin->calc(lval, rval);
		delete left;
		delete right;
		delete bin;
		return new Number(ans);
	} else if (ltype == VARIABLE && rtype == VARIABLE) {
		OPERNAME optype = bin->getType();
		Variable *lx = static_cast<Variable *>(left);
		Variable *rx = static_cast<Variable *>(right);
		if (optype == PLUS) {
			delete rx;
			delete bin;
			return new Mult(new Number(2), lx);
		} else if (optype == MINUS) {
			delete rx;
			delete lx;
			delete bin;
			return new Number(0);
		} else if (optype == MULT) {
			delete rx;
			delete bin;
			return new Pow(lx, new Number(2));
		} else if (optype == DIV) {
			delete lx;
			delete rx;
			delete bin;
			return new Number(1);
		}
	}
	return bin;
}

Node * Tree::processArithmetic(Node *cur_root) const
{
	if (cur_root == nullptr || cur_root->getNodeType() == TERM)
		return cur_root;
	Operator *op = static_cast<Operator *>(cur_root);
	if (op->isBinary()) {
		Binary *bin = static_cast<Binary *>(op);
		Node *left = bin->getLeftChild(), *right = bin->getRightChild();
		NODE_TYPE ltype = left->getNodeType(), rtype = right->getNodeType();
		if (ltype == OPERATOR && rtype == OPERATOR) {
			bin->setChilds(processArithmetic(left), processArithmetic(right));
			return bin;
		}
		if (ltype == TERM && rtype == TERM)
			return doArithmetic(bin);
		if (ltype == TERM) {
			Operator *rop = static_cast<Operator *>(right);
			if (rop->isBinary()) {
				bin->setRightChild(doArithmetic(static_cast<Binary *>(rop)));
				return doArithmetic(bin);
			}
		} else if (rtype == TERM) {
			Operator *lop = static_cast<Operator *>(left);
			if (lop->isBinary()) {
				bin->setLeftChild(doArithmetic(static_cast<Binary *>(lop)));
				return doArithmetic(bin);
			}
		}
	}
	return cur_root;
}

void Tree::simplify()
{
	root = processUseless(root);
	root = processArithmetic(root);
}

double Tree::value(Node *cur_root, double point) const
{
	if (cur_root->getNodeType() == TERM) {
		Term *term = static_cast<Term *>(cur_root);
		if (term->getTermType() == NUMBER)
			return static_cast<Number *>(term)->getValue();
		if (term->getTermType() == VARIABLE)
			return point;
		return 0;
	}
	Operator *op = static_cast<Operator *>(cur_root);
	if (op->isBinary()) {
		Binary *bin = static_cast<Binary *>(op);
		Node *left = bin->getLeftChild(), *right = bin->getRightChild();
		return bin->calc(value(left, point), value(right, point));
	}
	Function *func = static_cast<Function *>(op);
	Node *arg = func->getArg();
	return func->calc(value(arg, point));
}

double Tree::dvalue(Node *cur_root, double point) const
{
	if (cur_root->getNodeType() == TERM) {
		Term *term = static_cast<Term *>(cur_root);
		if (term->getTermType() == NUMBER) {
			return static_cast<Number *>(term)->getValue();
		}
		if (term->getTermType() == VARIABLE)
			return point;
		return 0;
	}
	Operator *op = static_cast<Operator *>(cur_root);
	if (op->isBinary()) {
		Binary *bin = static_cast<Binary *>(op);
		Node *left = bin->getLeftChild(), *right = bin->getRightChild();
		return bin->dcalc(dvalue(left, point), dvalue(right, point));
	}
	Function *func = static_cast<Function *>(op);
	Node *arg = func->getArg();
	return func->dcalc(dvalue(arg, point));
}

double Tree::value(double point) const
{
	return value(root, point);	
}

double Tree::dvalue(double point) const
{
	return dvalue(root, point);	
}
