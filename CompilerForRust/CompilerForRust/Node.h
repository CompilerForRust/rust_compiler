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
    "Program",                        //���ڵ�

    "FunctionDefinitions",            //�����������

    "FunctionDefinition",             //��������

    "Statements",                     //��������
    "Statement",                      //���

    "DeclarationStatement",           //�������
    "DeclarationRightStatement",    //���������ֵ
    "ExpressionStatement",
    "AssignmentExpression",
    "TypeExpression",

    "VariableDefinition",             //�ɱ����
    "DataType",
    "LiteralExpression",

    "Variable",

    "BlockExpression",
    //"LogicalOrExpression",
    "BinaryExpression", //��Ԫ���ʽ
    "LHS",
    "OP",
    "RHS",
    "GroupedExpression",
    "FunctionCall",
    "ContinueExpression",
    "BreakExpression",
    "IfExpression",
    "ReturnExpression",

    "Identifier",                     //��ʶ��

    "CHAR_STR_LITERAL",               //' [A-Za-z] ' | e
    "INTEGER_LITERAL",                //[0-9]*
    "FLOAT_LITERAL",                  //[0-9]*.[0-9]*
    "BOOLEAN_LITERAL",                //true | false

    "CycleExpression",                //ѭ��

    "WhileExpression",
    "ForExpression",
    "LoopExpression",

    "FunctionIdentifier",             //������
    "ParameterList",
    "CallParameterList",

    "AssignmentOperator",             // [*,/,-,+,<<,>>,%,&,^,|]?=
/*  ������ݹ�����
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
     "ConditionStatement",       //�������

     "COMMENT", //ע��

    "PRINTLN", // println!("{}", id);
/*  ������ݹ�����
* 
*   "LogicalOrExpressionE",            //������ݹ�
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
   "Token",                     //����token
};

enum class node_type
{
    Program,                        //���ڵ�

    FunctionDefinitions,            //�����������

    FunctionDefinition,             //��������

    Statements,                     //��������
    Statement,                      //���

    DeclarationStatement,           //�������
    DeclarationRightStatement,    //���������ֵ
    ExpressionStatement,
    AssignmentExpression,
    TypeExpression,

    VariableDefinition,             //�ɱ����
    DataType,
    LiteralExpression,

    Variable,

    BlockExpression,
    //LogicalOrExpression,
    BinaryExpression, //��Ԫ���ʽ
    LHS,
    OP,
    RHS,
    GroupedExpression,
    FunctionCall,
    ContinueExpression,
    BreakExpression,
    IfExpression,
    ReturnExpression,

    Identifier,                     //��ʶ��

    CHAR_STR_LITERAL,               //' [A-Za-z] ' | e
    INTEGER_LITERAL,                //[0-9]*
    FLOAT_LITERAL,                  //[0-9]*.[0-9]*
    BOOLEAN_LITERAL,                //true | false

    CycleExpression,                //ѭ��

    WhileExpression,
    ForExpression,
    LoopExpression,

    FunctionIdentifier,             //������
    ParameterList,
    CallParameterList,

    AssignmentOperator,             // [*,/,-,+,<<,>>,%,&,^,|]?=
/*  ������ݹ�����
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
    ConditionStatement,       //�������

    COMMENT, //ע��

    PRINTLN, // println!("{}", id);

/*   ������ݹ�����
* 
*    LogicalOrExpressionE,            //������ݹ�
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

    Token,                     //����token

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
