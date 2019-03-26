#include "postfix.h"

Postfix::Postfix(const Parser & parser)
{
	buildPostfix(parser.nodes);
}

void Postfix::buildPostfix(const vector<Node *> & nodes)
{
	stack<Operator *> op_stack;
	for (Node *cur: nodes) {
		NODE_TYPE nodetype = cur->getNodeType();
		if (nodetype == TERM) {
			postfix.push_back(cur);
		} else {
			Operator *op = static_cast<Operator *>(cur);
			OPERNAME optype = op->getType();
			if (optype == LBRACKET) {
				op_stack.push(op);
			} else if (optype == RBRACKET) {
				if (!op_stack.empty()) {
					while (!op_stack.empty() &&
						op_stack.top()->getType() != LBRACKET) 
					{
						postfix.push_back(op_stack.top());
						op_stack.pop();
					}
					op_stack.pop();
				}
			} else {
					if (!op_stack.empty()) {
						while(!op_stack.empty() && 
							op->priority() <= op_stack.top()->priority() &&
							op_stack.top()->getType() != LBRACKET)
						{
							postfix.push_back(op_stack.top());
							op_stack.pop();
						}
					}
					op_stack.push(op);
				}
		}
	}
	while (!op_stack.empty()) {
		postfix.push_back(op_stack.top());
		op_stack.pop();
	}
}

void Postfix::print() const
{
	for (Node *node: postfix)
		node->print();
	cout << endl;
}
