#include "nodes.h"


Node * Variable::diff() const
{
	return new Number(1);
}

Node * Const::diff() const
{
	return new Number(0);
}

Node * Plus::diff() const
{
	return new Plus(left->diff(), right->diff());
}

Node * Minus::diff() const
{
	return new Minus(left->diff(), right->diff());
}

Node * Mult::diff() const
{
	return new Plus(new Mult(left->diff(), right->copy()),
		new Mult(left->copy(), right->diff()));
}

Node * Div::diff() const
{
	Mult *mleft = new Mult(left->diff(), right->copy());
	Mult *mright = new Mult(left->copy(), right->diff());
	return new Div(new Minus(mleft, mright), 
		new Pow(right->copy(), new Number(2)));
}

Node * Pow::diff() const
{
	Node *u = left, *v = right;
	Div *div = new Div(new Mult(v->copy(), u->diff()), u->copy());
	Mult *mult = new Mult(v->diff(), new Ln(u->copy()));
	Plus *plus = new Plus(mult, div);
	return new Mult(new Pow(u->copy(), v->copy()), plus);
}

Node * Sin::diff() const
{
	return new Mult(arg->diff(), new Cos(arg->copy()));
}

Node * Cos::diff() const
{
	Mult *mult = new Mult(arg->diff(), new Sin(arg->copy()));
	return new Minus(new Number(0), mult);
}

Node * Tan::diff() const
{
	return new Div(arg->diff(), new Pow(new Cos(arg->copy()), 
		new Number(2)));
}

Node * Cot::diff() const
{
	return new Minus(new Number(0), new Div(arg->diff(), 
		new Pow(new Cos(arg->copy()), new Number(2))));
}


Node * Exp::diff() const
{
	return new Mult(arg->diff(), new Exp(arg->copy()));
}

Node * Ln::diff() const
{
	return new Div(arg->diff(), arg->copy());
}
