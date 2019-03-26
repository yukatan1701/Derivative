#ifndef __NODES_H__
#define __NODES_H__

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

enum NODE_TYPE { OPERATOR, TERM };
enum OPER_TYPE { FUNCTION, BINARY };

enum OPERNAME {
	SIN, COS, TAN, COT, EXP, LN,
	PLUS, MINUS, MULT, DIV, POW,
	LBRACKET, RBRACKET
};

const vector<string> OPERTEXT {
	"sin", "cos", "tan", "cot", "exp", "ln",
	"+", "-", "*", "/", "^",
	"(", ")"
};

const int PRIORITY[] = {
	0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 2,
	13, 13
};

class Node {
public:
	Node() {}
	virtual ~Node() {}
	virtual Node *differentiate() const = 0;
	virtual void print() const = 0;
	virtual NODE_TYPE getNodeType() const = 0;
	virtual Node *deepCopy() const = 0;
};

class Operator: public Node {
	OPERNAME optype;
protected:	
	Node *left;
	Node *right;
public:
	Operator(OPERNAME optype): optype(optype) { left = nullptr; right = nullptr; }
	NODE_TYPE getNodeType() const { return OPERATOR; }
	OPERNAME getType() const { return optype; }
	int priority() const { return PRIORITY[optype]; }
	void print() const { cout << OPERTEXT[optype]; }
	Node *getLeftChild() const { return left; }
	Node *getRightChild() const { return right; }
	void setLeftChild(Node *l_ch) { left = l_ch; }
	void setRightChild(Node *r_ch) { right = r_ch; }
	void setChilds(Node *lch, Node *rch) { left = lch; right = rch; }
	virtual bool isBinary() const { return true; }
};

class Term: public Node {
public:
	NODE_TYPE getNodeType() const { return TERM; }
};

class Const: public Term {
public:	
	Node *differentiate() const;
};

class Number: public Const {
	double value; 
public:
	Number(double value): value(value) {}
	double getValue() const { return value; }
	void print() const { cout << "{" << value << "}"; }
	Node *deepCopy() const { return new Number(value); }
};

class Nominal: public Const {
	string name;
public:
	Nominal(string name): name(name) {}
	string getName() const { return name; }
	void print() const { cout << "{" <<  name << "}"; }
	Node *deepCopy() const { return new Nominal(name); }
};

class Variable: public Term {
public:
	void print() const { cout << "[x]"; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Variable(); }
};

class Binary: public Operator {
public:
	Binary(OPERNAME optype): Operator(optype) {}
	bool isBinary() const { return true; }
};

class Function: public Operator {
public:
	Function(OPERNAME optype): Operator(optype) {}
	bool isBinary() const { return false; }
};

class Sin: public Function {
public:
	Sin(): Function(SIN) {}
	Sin(Node *r): Function(SIN) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Sin(right->deepCopy()); }
};

class Cos: public Function {
public:
	Cos(): Function(COS) {}
	Cos(Node *r): Function(COS) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Cos(right->deepCopy()); }
};

class Tan: public Function {
public:
	Tan(): Function(TAN) {}
	Tan(Node *r): Function(TAN) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Tan(right->deepCopy()); }
};

class Cot: public Function {
public:
	Cot(): Function(COT) {}
	Cot(Node *r): Function(COT) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Cot(right->deepCopy()); }
};

class Exp: public Function {
public:
	Exp(): Function(EXP) {}
	Exp(Node *r): Function(EXP) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Exp(right->deepCopy()); }
};

class Ln: public Function {
public:
	Ln(): Function(LN) {}
	Ln(Node *r): Function(LN) { right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Ln(right->deepCopy()); }
};

class Pow: public Binary {
public:
	Pow(): Binary(POW) {}
	Pow(Node *l, Node *r): Binary(POW) { left = l; right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Pow(left->deepCopy(), right->deepCopy()); }
};

class Plus: public Binary {
public:
	Plus(): Binary(PLUS) {}
	Plus(Node *l, Node *r): Binary(PLUS) { left = l; right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Plus(left->deepCopy(), right->deepCopy()); }
};

class Minus: public Binary {
public:
	Minus(): Binary(MINUS) {}
	Minus(Node *l, Node *r): Binary(MINUS) { left = l; right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Minus(left->deepCopy(), right->deepCopy()); }
};

class Mult: public Binary {
public:
	Mult(): Binary(MULT) {}
	Mult(Node *l, Node *r): Binary(MULT) { left = l; right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Mult(left->deepCopy(), right->deepCopy()); }
};

class Div: public Binary {
public:
	Div(): Binary(DIV) {}
	Div(Node *l, Node *r): Binary(DIV) { left = l; right = r; }
	Node *differentiate() const;
	Node *deepCopy() const { return new Div(left->deepCopy(), right->deepCopy()); }
};

class LBracket: public Operator {
public:
	LBracket(): Operator(LBRACKET) {}
	Node *differentiate() const { return nullptr; }
	Node *deepCopy() const { return nullptr; }
};

class RBracket: public Operator {
public:
	RBracket(): Operator(RBRACKET) {}
	Node *differentiate() const { return nullptr; }
	Node *deepCopy() const { return nullptr; }
};

#endif
