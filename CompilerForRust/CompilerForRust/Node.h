#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>


using namespace std;
using namespace llvm;

static map<string, int> initMap() {
    map<string, int>BinopPrecedence;
    BinopPrecedence["||"] = 0;
    BinopPrecedence["&&"] = 10;
    BinopPrecedence["|"] = 20;
    BinopPrecedence["^"] = 30;
    BinopPrecedence["&"] = 40;
    BinopPrecedence["=="] = 50;
    BinopPrecedence["!="] = 50;
    BinopPrecedence["<"] = 60;
    BinopPrecedence["<="] = 60;
    BinopPrecedence[">"] = 60;
    BinopPrecedence[">="] = 60;
    BinopPrecedence["<<"] = 70;
    BinopPrecedence[">>"] = 70;
    BinopPrecedence["+"] = 80;
    BinopPrecedence["-"] = 80;
    BinopPrecedence["*"] = 90;
    BinopPrecedence["/"] = 90;
    BinopPrecedence["%"] = 90;
    BinopPrecedence["!"] = 100;
    return BinopPrecedence;
};
static const map<string, int>BinopPrecedence=initMap();



static const char* nodeTypeList[] = {
    "Program",                        //根节点

    "FunctionDefinitions",            //多个函数定义

    "FunctionDefinition",             //单个函数

    "Statements",                     //函数过程
    "Statement",                      //语句

    "DeclarationStatement",           //声明语句
    "DeclarationRightStatement",    //声明语句右值
    "ExpressionStatement",
    "AssignmentExpression",
    "TypeExpression",

    "VariableDefinition",             //可变变量
    "DataType",
    "LiteralExpression",

    "Variable",

    "BlockExpression",
    //"LogicalOrExpression",
    "BinaryExpression", //二元表达式
    "LHS",
    "OP",
    "RHS",
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
/*  消除左递归弃用
* 
*   "LogicalAndExpression",           // &&
*   "InclusiveOrExpression",          // |
*   "ExclusiveOrExpression",          // ^
*   "AndExpression",                  // &
*   "EqualityExpression",             // == | !=
*   "RelationalExpression",           // [<,>]=?
*   "ShiftExpression",                // << | >>
*   "AdditiveExpression",             // + | -
*   "MultiplicativeExpression",       // * | / | %
*   "NotExpression",                  // !
*/
    "PrimaryExpression",              // (?)?
     "ConditionStatement",       //条件语句

     "COMMENT", //注释

    "PRINTLN", // println!("{}", id);
/*  消除左递归弃用
* 
*   "LogicalOrExpressionE",            //消除左递归
*   "LogicalAndExpressionE",
*   "InclusiveOrExpressionE",
*   "ExclusiveOrExpressionE",
*   "AndExpressionE",
*   "EqualityExpressionE",
*   "RelationalExpressionE",
*   "ShiftExpressionE",
*   "AdditiveExpressionE",
*   "MultiplicativeExpressionE",
*
*/
   "Token",                     //保存token
};

enum class node_type
{
    Program,                        //根节点

    FunctionDefinitions,            //多个函数定义

    FunctionDefinition,             //单个函数

    Statements,                     //函数过程
    Statement,                      //语句

    DeclarationStatement,           //声明语句
    DeclarationRightStatement,    //声明语句右值
    ExpressionStatement,
    AssignmentExpression,
    TypeExpression,

    VariableDefinition,             //可变变量
    DataType,
    LiteralExpression,

    Variable,

    BlockExpression,
    //LogicalOrExpression,
    BinaryExpression, //二元表达式
    LHS,
    OP,
    RHS,
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
/*  消除左递归弃用
* 
*    LogicalAndExpression,           // &&
*    InclusiveOrExpression,          // |
*    ExclusiveOrExpression,          // ^
*    AndExpression,                  // &
*    EqualityExpression,             // == | !=
*    RelationalExpression,           // [<,>]=?
*    ShiftExpression,                // << | >>
*    AdditiveExpression,             // + | -
*    MultiplicativeExpression,       // * | / | %
*    NotExpression,                  // !
*/    
    PrimaryExpression,              // (?)?
    ConditionStatement,       //条件语句

    COMMENT, //注释

    PRINTLN, // println!("{}", id);

/*   消除左递归弃用
* 
*    LogicalOrExpressionE,            //消除左递归
*    LogicalAndExpressionE,
*    InclusiveOrExpressionE,
*    ExclusiveOrExpressionE,
*    AndExpressionE,
*    EqualityExpressionE,
*    RelationalExpressionE,
*    ShiftExpressionE,
*    AdditiveExpressionE,
*    MultiplicativeExpressionE,
*/

    Token,                     //保存token

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
    void Init();
    void print();
    Value* codegen();
    Type* getType(string returnVal);
};
