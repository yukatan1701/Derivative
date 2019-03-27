#include <iostream>
#include <string>
#include <vector>
#include "tree.h"
//#include "nodes.h"
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
	/*if (argc < 2) {
		cout << "Too few arguments." << endl;
		return 0;
	}*/
	Parser parser(readFormula());
	//parser.print();
	Postfix postfix(parser);
	postfix.print();
	cout << endl;
	Tree tree(postfix);
	tree.print();
	Tree der = tree.getDerivative();
	der.print();
	double x;
	cin >> x;
	//double x = atof(argv[1]);
	cout << der.value(x) << endl;
	cout << tree.dvalue(x) << endl;
	return 0;
}
