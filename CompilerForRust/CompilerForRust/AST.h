#pragma once
#include "Node.h"
#include "Log.h"

class AST
{
public:
	unique_ptr<Node> tree;

public:
	AST();
	~AST();

	void printRecursive(unique_ptr<Node> &current_node, size_t level);
	void print();
};