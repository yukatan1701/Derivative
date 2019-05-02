#include "parser.h"

Number * Parser::readNumber(const string & input, unsigned int & i)
{
	char ch = input[i];
	string str_num;
	bool has_dot = false;
	while (i < input.size() && (isdigit(ch) || ch == '.')) {
		if (ch == '.') {
			if (has_dot)
				cout << "Too many dots for number. Ignore." << endl;
			else
				has_dot = true;
		}
		str_num += ch;
		ch = input[++i];
	}
	i--;
	double num = atof(str_num.c_str());
	return new Number(num);
}

Operator * Parser::getOperatorByName(string name)
{
	if (name == "sin")
		return new Sin();
	if (name == "cos")
		return new Cos();
	if (name == "tan")
		return new Tan();
	if (name == "cot")
		return new Cot();
	if (name == "exp")
		return new Exp();
	if (name == "ln")
		return new Ln();
	if (name == "+")
		return new Plus();
	if (name == "-")
		return new Minus();
	if (name == "*")
		return new Mult();
	if (name == "/")
		return new Div();
	if (name == "(")
		return new LBracket();
	if (name == ")")
		return new RBracket();
	if (name == "^")
		return new Pow();
	cout << "Invalid name." << endl;
	return nullptr;
}

Node * Parser::wordToNode(string input, unsigned int & i)
{
	char ch = input[i];
	string name;
	while (i < input.size() && isalpha(ch)) {
		name += ch;
		ch = input[++i];
	}
	i--;
	for (string oper: OPERTEXT) {
		if (oper == name) {
			return getOperatorByName(name);
		}
	}
	if (name == "at") {
		has_at = true;
		return nullptr;
	}
	return new Nominal(name);
}

bool Parser::isSign(char ch)
{
	if (ch == '+' || ch == '-' || ch == '/' || 
		ch == '*' || ch == '(' || ch == ')' || ch == '^')
		return true;
	return false;
}

Parser::Parser(string formula)
{
	has_at = false;
	for (unsigned int i = 0; i < formula.size(); i++) {
		char ch = formula[i];
		if (ch == ' ')
			continue;
		if (isdigit(ch)) {
			Number *num = readNumber(formula, i);
			if (has_at) {
				point = num->getValue();
				delete num;
				break;
			} else {
				nodes.push_back(num);
			}
		} else if (ch == 'x') {
			Variable *var = new Variable();
			nodes.push_back(var);
		} else if (isSign(ch)) {
			string s_ch;
			s_ch += ch;
			Operator *op = getOperatorByName(s_ch);
			nodes.push_back(op);
		} else if (isalpha(ch)) {
			Node *node = wordToNode(formula, i);
			if (!has_at)
				nodes.push_back(node);
		} else {
			cout << "Invalid char: " << ch << ". Ignore." << endl;
		}
	}
}

void Parser::print() const
{
	for (Node *node: nodes)
		node->print();
	cout << endl;
}

Parser::~Parser()
{
	for (Node *node: nodes)
		delete node;
}
