#pragma once
#include <iostream>
#include <vector>

using namespace std;

enum class node_type
{
  Program,

  FunctionDeclarations,
  FunctionDefinitions,
  Main,

  FunctionDeclaration,
  FunctionDefinition,

  Statements,
  Statement,

  DeclarationStatement,
  ExpressionStatement,
  AssignmentExpression,
  TypeExpression,

  VariableDefinition,
  DataType,
  LiteralExpression,

  Variable,

  BlockExpression,
  LogicalOrExpression,
  GroupedExpression,
  FunctionCall,
  LoopExpression,
  ContinueExpression,
  BreakExpression,
  IfExpression,
  ReturnExpression,

  Identifier,

  CHAR_STR_LITERAL,
  INTEGER_LITERAL,
  FLOAT_LITERAL,
  BOOLEAN_LITERAL,

  CycleExpression,

  WhileExpression,
  ForExpression,
  LoopExpression,

  IfExpression,

  FunctionIdentifier,
  ParameterList,
  CallParameterList,

  AssignmentOperator,
  LogicalAndExpression,
  InclusiveOrExpression,
  ExclusiveOrExpression,
  AndExpression,
  EqualityExpression,
  RelationalExpression,
  ShiftExpression,
  AdditiveExpression,
  MultiplicativeExpression,
  NotExpression,
  PrimaryExpression
};
class Node
{
public:
  string value;
  node_type type;
  vector<unique_ptr<Node>> childNodes;

public:
  Node(string value, node_type type, vector<unique_ptr<Node>> childNodes);
  Node();
  ~Node();
  void addChildNode(unique_ptr<Node> childNode);
};