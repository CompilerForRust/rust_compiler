#include "AST.h"

AST::AST()
{
	tree = nullptr;
}
AST::~AST() {}

void AST::printRecursive(unique_ptr<Node> &current_node, size_t level)
{
	if (current_node == nullptr)
	{
		return;
	}

	for (int i = 0; i < level; ++i)
	{
		cout_log("  ");
	}

	cout_log("+-");

	//根据节点类型输出
	switch (current_node->type)
	{
	case node_type::Variable:
		cout_log("Variable (");
		cout_log(current_node->value);
		cout_log(")");
		break;
	case node_type::Identifier:
		cout_log("Identifier (");
		cout_log(current_node->value);
		cout_log(")");
		break;
	case node_type::CHAR_STR_LITERAL:
		cout_log("CHAR_STR_LITERAL (");
		cout_log(current_node->value);
		cout_log(")");
		break;
	case node_type::INTEGER_LITERAL:
		cout_log("INTEGER_LITERAL (");
		cout_log(current_node->value);
		cout_log(")");
		break;
	case node_type::FLOAT_LITERAL:
		cout_log("FLOAT_LITERAL (");
		cout_log(current_node->value);
		cout_log(")");
		break;
	default:
		cout_log(nodeTypeList[(int)current_node->type]);
		break;
	}
	endl_log();

	for (unsigned i = 0; i < current_node->childNodes.size(); i++)
	{
		printRecursive(current_node->childNodes[i], level + 1);
	}
}

void AST::print()
{
	cout_log("------ AST ------");
	endl_log();
	printRecursive(tree, 0);
	endl_log();
	cout << "AST writed successfully" << endl;
}