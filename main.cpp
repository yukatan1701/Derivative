#include <iostream>
#include <string>
#include <vector>
#include "tree.h"
#include "parser.h"
#include "postfix.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

string readFormula()
{
	string str;
	int c;
	while ((c = cin.get()) != '\n')
		str.push_back((char) c);
	return str;
}

int main(int argc, char *argv[])
{
	cout << "Format: <formula> [at <point>]" << endl;
	Parser parser(readFormula());
	//parser.print();
	Postfix postfix(parser);
	//postfix.print();
	cout << endl;
	Tree tree(postfix);
	//tree.print();
	Tree der = tree.getDerivative();
	cout << "Derivative (after simplifying):" << endl;
	der.print();
	if (tree.hasPoint()) {
		cout << "Substitution: " << der.value() << endl;
		cout << "At point: " << tree.dvalue() << endl;
	}
	return 0;
}
