#include "nodes.h"


Node * Variable::differentiate() const
{
	return new Number(1);
}

Node * Const::differentiate() const
{
	return new Number(0);
}

Node * Plus::differentiate() const
{
	Node *left_branch = left->differentiate();
	Node *right_branch = right->differentiate();
	Plus *plus = new Plus(left_branch, right_branch);
	return plus;
}

Node * Minus::differentiate() const
{
	Node *left_branch = left->differentiate();
	Node *right_branch = right->differentiate();
	Minus *minus = new Minus(left_branch, right_branch);
	return minus;
}

Node * Mult::differentiate() const
{
	Node *u = left;
	Node *v = right;
	Node *du = u->differentiate();
	Node *dv = v->differentiate();
	Mult *mleft = new Mult(du, v->deepCopy());
	Mult *mright = new Mult(u->deepCopy(), dv);
	Plus *plus = new Plus(mleft, mright);
	return plus;
}

Node * Div::differentiate() const
{
	Node *u = left;
	Node *v = right;	
	Node *du = u->differentiate();
	Node *dv = v->differentiate();
	Mult *mleft = new Mult(du, v->deepCopy());
	Mult *mright = new Mult(u->deepCopy(), dv);
	Minus *minus = new Minus(mleft, mright);
	Number *deg = new Number(2);
	Pow *pow = new Pow(v->deepCopy(), deg);
	Div *div = new Div(minus, pow);
	return div;
}

Node * Pow::differentiate() const
{
	Node *u = left;
	Node *du = u->differentiate();
	Node *v = right;
	Number *deg = static_cast<Number *>(v);
	Number *coef = new Number(deg->getValue());
	Number *new_deg = new Number(deg->getValue() - 1);
	Pow *pow = new Pow(u->deepCopy(), new_deg);
	Mult *pow_mult = new Mult(pow, du);
	Mult *coef_mult = new Mult(coef, pow_mult);
	return coef_mult;
}

Node * Sin::differentiate() const
{
	Node *u = right;
	Cos *cos = new Cos(u->deepCopy());
	Node *du = u->differentiate();
	Mult *mult = new Mult(du, cos);
	return mult;
}

Node * Cos::differentiate() const
{
	Node *u = right;
	Sin *sin = new Sin(u->deepCopy());
	Node *du = u->differentiate();
	Mult *mult = new Mult(du, sin);
	Number *zero = new Number(0);
	Minus *minus = new Minus(zero, mult);
	return minus;
}

Node * Tan::differentiate() const
{
	return nullptr;
}

Node * Cot::differentiate() const
{
	return nullptr;
}


Node * Exp::differentiate() const
{
	return nullptr;
}

Node * Ln::differentiate() const
{
	return nullptr;
}
