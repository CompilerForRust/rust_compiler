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
		cout << " ";
	}

	//根据节点类型输出
	switch (current_node->type)
	{
	}

	for (unsigned i = 0; i < current_node->childNodes.size(); i++)
	{
		printRecursive(current_node->childNodes[i], level + 1);
	}
}

void AST::print()
{
	printRecursive(tree, 0);
}