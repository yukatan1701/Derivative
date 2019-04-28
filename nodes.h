#ifndef __NODES_H__
#define __NODES_H__

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

enum NODE_TYPE { OPERATOR, TERM };
enum OPER_TYPE { FUNCTION, BINARY };
enum TERM_TYPE { VARIABLE, NOMINAL, NUMBER };

enum OPERNAME {
	PLUS, MINUS, MULT, DIV, POW,
	SIN, COS, TAN, COT, EXP, LN,
	LBRACKET, RBRACKET
};

const vector<string> OPERTEXT {
	"+", "-", "*", "/", "^",
	"sin", "cos", "tan", "cot", "exp", "ln",
	"(", ")"
};

const int PRIORITY[] = {
	0, 0, 1, 1, 2,
	3, 3, 3, 3, 3, 3,
	13, 13
};

class Node {
public:
	Node() {}
	virtual ~Node() {}
	virtual Node *diff() const = 0;
	virtual void print() const = 0;
	virtual NODE_TYPE getNodeType() const = 0;
	virtual Node *copy() const = 0;
	virtual double diffAt(double point) const = 0;
	virtual double calcBranch(double point) const = 0;
};

class Operator: public Node {
	OPERNAME optype;
public:
	Operator(OPERNAME optype): optype(optype) {}
	NODE_TYPE getNodeType() const { return OPERATOR; }
	OPERNAME getType() const { return optype; }
	int priority() const { return PRIORITY[optype]; }
	void print() const { cout << OPERTEXT[optype]; }
	virtual bool isBinary() const { return true; }
};

class Term: public Node {
public:
	NODE_TYPE getNodeType() const { return TERM; }
	virtual TERM_TYPE getTermType() const = 0;
};

class Const: public Term {
public:	
	Node *diff() const;
	double diffAt(double point) const { return 0; }
};

class Number: public Const {
	double value; 
public:
	Number(double value): value(value) {}
	double getValue() const { return value; }
	void print() const { cout << value; }
	Node *copy() const { return new Number(value); }
	TERM_TYPE getTermType() const { return NUMBER; }
	double calcBranch(double point) const { return value; }
};

class Nominal: public Const {
	string name;
public:
	Nominal(string name): name(name) {}
	string getName() const { return name; }
	void print() const { cout << name; }
	Node *copy() const { return new Nominal(name); }
	TERM_TYPE getTermType() const { return NOMINAL; }
	double calcBranch(double point) const { return -1; }
};

class Variable: public Term {
public:
	void print() const { cout << "x"; }
	Node *diff() const;
	Node *copy() const { return new Variable(); }
	TERM_TYPE getTermType() const { return VARIABLE; }
	double diffAt(double point) const { return 1; }
	double calcBranch(double point) const { return point; }
};

class Binary: public Operator {
protected:	
	Node *left;
	Node *right;
public:
	Binary(OPERNAME optype): Operator(optype) { left = nullptr; right = nullptr; }
	void setChilds(Node *l, Node *r) { left = l; right = r; }
	void setLeftChild(Node *l) { left = l; }
	void setRightChild(Node *r) { right = r; }
	Node *getLeftChild() const { return left; }
	Node *getRightChild() const { return right; }
	bool isBinary() const { return true; }
	virtual double calc(double x, double y) const { return 0; }
};

class Function: public Operator {
protected:
	Node *arg;
public:
	Function(OPERNAME optype): Operator(optype) { arg = nullptr; }
	void setArg(Node *a) { arg = a; }
	Node *getArg() const { return arg; }
	bool isBinary() const { return false; }
	virtual double calc(double val) const { return 0; }
};

class Sin: public Function {
public:
	Sin(): Function(SIN) {}
	Sin(Node *r): Function(SIN) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Sin(arg->copy()); }
	double calc(double val) const { return sin(val); }
	double diffAt(double point) const
		{ return arg->diffAt(point) * cos(arg->calcBranch(point)); }
	double calcBranch(double point) const { return sin(arg->calcBranch(point)); }
};

class Cos: public Function {
public:
	Cos(): Function(COS) {}
	Cos(Node *r): Function(COS) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Cos(arg->copy()); }
	double calc(double val) const { return cos(val); }
	double diffAt(double point) const
		{ return arg->diffAt(point) * -sin(arg->calcBranch(point)); }
	double calcBranch(double point) const { return cos(arg->calcBranch(point)); }
};

class Tan: public Function {
public:
	Tan(): Function(TAN) {}
	Tan(Node *r): Function(TAN) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Tan(arg->copy()); }
	double calc(double val) const { return tan(val); }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return tan(arg->calcBranch(point)); }
};

class Cot: public Function {
public:
	Cot(): Function(COT) {}
	Cot(Node *r): Function(COT) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Cot(arg->copy()); }
	double calc(double val) const { return 1 / tan(val); }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return 1 / tan(arg->calcBranch(point)); }
};

class Exp: public Function {
public:
	Exp(): Function(EXP) {}
	Exp(Node *r): Function(EXP) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Exp(arg->copy()); }
	double calc(double val) const { return exp(val); }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return exp(arg->calcBranch(point)); }
};

class Ln: public Function {
public:
	Ln(): Function(LN) {}
	Ln(Node *r): Function(LN) { arg = r; }
	Node *diff() const;
	Node *copy() const { return new Ln(arg->copy()); }
	double calc(double val) const { return log(val); }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return log(arg->calcBranch(point)); }
};

class Pow: public Binary {
public:
	Pow(): Binary(POW) {}
	Pow(Node *l, Node *r): Binary(POW) { left = l; right = r; }
	Node *diff() const;
	Node *copy() const { return new Pow(left->copy(), right->copy()); }
	double calc(double x, double y) const { return pow(x, y); }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return pow(left->calcBranch(point), right->calcBranch(point)); }
};

class Plus: public Binary {
public:
	Plus(): Binary(PLUS) {}
	Plus(Node *l, Node *r): Binary(PLUS) { left = l; right = r; }
	Node *diff() const;
	Node *copy() const { return new Plus(left->copy(), right->copy()); }
	double calc(double x, double y) const { return x + y; }
	double diffAt(double point) const
		{ return left->diffAt(point) + right->diffAt(point); }
	double calcBranch(double point) const
		{ return left->calcBranch(point) + right->calcBranch(point); }
};

class Minus: public Binary {
public:
	Minus(): Binary(MINUS) {}
	Minus(Node *l, Node *r): Binary(MINUS) { left = l; right = r; }
	Node *diff() const;
	Node *copy() const { return new Minus(left->copy(), right->copy()); }
	double calc(double x, double y) const { return x - y; }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return left->calcBranch(point) - right->calcBranch(point); }
};

class Mult: public Binary {
public:
	Mult(): Binary(MULT) {}
	Mult(Node *l, Node *r): Binary(MULT) { left = l; right = r; }
	Node *diff() const;
	Node *copy() const { return new Mult(left->copy(), right->copy()); }
	double calc(double x, double y) const { return x * y; }
	double diffAt(double point) const
		{ return left->diffAt(point) * right->calcBranch(point) +
		right->diffAt(point) * left->calcBranch(point); }
	double calcBranch(double point) const
		{ return left->calcBranch(point) * right->calcBranch(point); }
};

class Div: public Binary {
public:
	Div(): Binary(DIV) {}
	Div(Node *l, Node *r): Binary(DIV) { left = l; right = r; }
	Node *diff() const;
	Node *copy() const { return new Div(left->copy(), right->copy()); }
	double calc(double x, double y) const { return x / y; }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const
		{ return left->calcBranch(point) / right->calcBranch(point); }
};

class LBracket: public Operator {
public:
	LBracket(): Operator(LBRACKET) {}
	Node *diff() const { return nullptr; }
	Node *copy() const { return nullptr; }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const { return 0; }
};

class RBracket: public Operator {
public:
	RBracket(): Operator(RBRACKET) {}
	Node *diff() const { return nullptr; }
	Node *copy() const { return nullptr; }
	double diffAt(double point) const { return 0; }
	double calcBranch(double point) const { return 0; }
};

#endif
