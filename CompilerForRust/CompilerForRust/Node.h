#pragma once
#include <iostream>
#include <vector>

using namespace std;
static const char* nodeTypeList[] = {
    "Program",                        //根节点

    "FunctionDefinitions",            //多个函数定义
    "Main",

    "FunctionDefinition",             //单个函数

    "Statements",                     //函数过程
    "Statement",                      //语句

    "DeclarationStatement",           //声明语句
    "ExpressionStatement",
    "AssignmentExpression",
    "TypeExpression",

    "VariableDefinition",             //可变变量
    "DataType",
    "LiteralExpression",

    "Variable",

    "BlockExpression",
    "LogicalOrExpression",
    "GroupedExpression",
    "FunctionCall",
    "ContinueExpression",
    "BreakExpression",
    "IfExpression",
    "ReturnExpression",

    "Identifier",                     //标识符

    "CHAR_STR_LITERAL",               //' [A-Za-z] ' | e
    "INTEGER_LITERAL",                //[0-9]*
    "FLOAT_LITERAL",                  //[0-9]*.[0-9]*
    "BOOLEAN_LITERAL",                //true | false

    "CycleExpression",                //循环

    "WhileExpression",
    "ForExpression",
    "LoopExpression",

    "FunctionIdentifier",             //函数名
    "ParameterList",
    "CallParameterList",

    "AssignmentOperator",             // [*,/,-,+,<<,>>,%,&,^,|]?=
    "LogicalAndExpression",           // &&
    "InclusiveOrExpression",          // |
    "ExclusiveOrExpression",          // ^
    "AndExpression",                  // &
    "EqualityExpression",             // == | !=
    "RelationalExpression",           // [<,>]=?
    "ShiftExpression",                // << | >>
    "AdditiveExpression",             // + | -
    "MultiplicativeExpression",       // * | / | %
    "NotExpression",                  // !
    "PrimaryExpression",              // (?)?

    "PRINTLN", // println!("{}", id);
};

enum class node_type
{
    Program,                        //根节点

    FunctionDefinitions,            //多个函数定义
    Main,
         
    FunctionDefinition,             //单个函数

    Statements,                     //函数过程
    Statement,                      //语句

    DeclarationStatement,           //声明语句
    ExpressionStatement,
    AssignmentExpression,
    TypeExpression,

    VariableDefinition,             //可变变量
    DataType,
    LiteralExpression,

    Variable,

    BlockExpression,
    LogicalOrExpression,
    GroupedExpression,
    FunctionCall,
    ContinueExpression,
    BreakExpression,
    IfExpression,
    ReturnExpression,

    Identifier,                     //标识符

    CHAR_STR_LITERAL,               //' [A-Za-z] ' | e
    INTEGER_LITERAL,                //[0-9]*
    FLOAT_LITERAL,                  //[0-9]*.[0-9]*
    BOOLEAN_LITERAL,                //true | false

    CycleExpression,                //循环

    WhileExpression,
    ForExpression,
    LoopExpression,

    FunctionIdentifier,             //函数名
    ParameterList,
    CallParameterList,

    AssignmentOperator,             // [*,/,-,+,<<,>>,%,&,^,|]?=
    LogicalAndExpression,           // &&
    InclusiveOrExpression,          // |
    ExclusiveOrExpression,          // ^
    AndExpression,                  // &
    EqualityExpression,             // == | !=
    RelationalExpression,           // [<,>]=?
    ShiftExpression,                // << | >>
    AdditiveExpression,             // + | -
    MultiplicativeExpression,       // * | / | %
    NotExpression,                  // !
    PrimaryExpression,              // (?)?

    PRINTLN, // println!("{}", id);
};
class Node
{
public:
    string value;
    node_type type;
    vector<unique_ptr<Node>> childNodes;

public:
    Node(const string value, node_type type);
    Node(const string value, node_type type, vector<unique_ptr<Node>> childNodes);
    Node();
    ~Node();
    void addChildNode(unique_ptr<Node> childNode);
};