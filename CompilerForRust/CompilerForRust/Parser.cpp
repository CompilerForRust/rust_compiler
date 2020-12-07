#include "Parser.h"

Parser::Parser(const string file_path)
{
	lex = new Lexer(file_path);
	ast = new AST();
}

Parser::~Parser()
{
	delete lex;
	delete ast;
}

void Parser::parse()
{
	lex->split(); // Receiving tokens
	lex->print(); // Write tokens to the log file

	ast->tree = Program(); // Call Recursive descent parser
	ast->print(); // Write AST to the log file
	
}

//Node* Parser::getAst()
//{
//	return ast->tree;
//}


string Parser::eat(token_type type)
{
	if (!tryEat(type)) {
		string temp = tokenTypeList[(int)type];
		error(temp + " expected!");
	}

	string value = lex->current_token_value();
	lex->next_token();
	return value;
}

bool Parser::tryEat(token_type type)
{
	return lex->current_token_type() == type;
}

unique_ptr<Node> Parser::Program(){
	auto functionDefinitionsChild = FunctionDefinitions();
	auto mainChild = Main();
	unique_ptr<Node> programNode(new Node("", node_type::Program));
	programNode->addChildNode(move(functionDefinitionsChild));
	programNode->addChildNode(move(mainChild));
	return programNode;
}

unique_ptr<Node> Parser::FunctionDefinitions() {
	if (tryEat(token_type::FN))
	{
		auto functionDefinitionChild = FunctionDefinition();
		auto functionDefinitionsChild = FunctionDefinitions();
		unique_ptr<Node> functionDefinitionsNode(new Node("", node_type::FunctionDefinitions));
		functionDefinitionsNode->addChildNode(move(functionDefinitionChild));
		functionDefinitionsNode->addChildNode(move(functionDefinitionsChild));
		return functionDefinitionsNode;
	}
	else
		return nullptr;
}
unique_ptr<Node> Parser::Main(){ return nullptr; }
unique_ptr<Node> Parser::FunctionDefinition()
{ 
	eat(token_type::FN);
	auto functionIdentifierChild = FunctionIdentifier();
	eat(token_type::LPAR);
	auto parameterListChild = ParameterList();
	eat(token_type::RPAR);
	if (tryEat(token_type::RETURNTYPE)) {
		eat(token_type::RETURNTYPE);
		auto dataTypeChild = DataType();
		auto blockExpressionChlid = BlockExpression();
		unique_ptr<Node> functionDefinitionNode(new Node("", node_type::FunctionIdentifier));
		functionDefinitionNode->addChildNode(move(functionIdentifierChild));
		functionDefinitionNode->addChildNode(move(parameterListChild));
		functionDefinitionNode->addChildNode(move(dataTypeChild));
		functionDefinitionNode->addChildNode(move(blockExpressionChlid));
		return functionDefinitionNode;
	}
	else {
		auto blockExpressionChild = BlockExpression();
		unique_ptr<Node> functionDefinitionNode(new Node("", node_type::FunctionIdentifier));
		functionDefinitionNode->addChildNode(move(functionIdentifierChild));
		functionDefinitionNode->addChildNode(move(parameterListChild));
		functionDefinitionNode->addChildNode(move(blockExpressionChild));
		return functionDefinitionNode;
	}
}
unique_ptr<Node> Parser::FunctionDefinition(){ return nullptr; }
unique_ptr<Node> Parser::Statements(){ return nullptr; }
unique_ptr<Node> Parser::Statement(){ return nullptr; }
unique_ptr<Node> Parser::DeclarationStatement(){ return nullptr; }
unique_ptr<Node> Parser::ExpressionStatement(){ return nullptr; }
unique_ptr<Node> Parser::AssignmentExpression(){ return nullptr; }
unique_ptr<Node> Parser::TypeExpression(){ return nullptr; }
unique_ptr<Node> Parser::VariableDefinition(){ return nullptr; }
unique_ptr<Node> Parser::DataType(){
	return nullptr; 
}
unique_ptr<Node> Parser::LiteralExpression(){ return nullptr; }
unique_ptr<Node> Parser::Variable(){ return nullptr; }
unique_ptr<Node> Parser::BlockExpression(){ return nullptr; }
unique_ptr<Node> Parser::LogicalOrExpression(){ return nullptr; }
unique_ptr<Node> Parser::GroupedExpression(){ return nullptr; }
unique_ptr<Node> Parser::FunctionCall(){ return nullptr; }
unique_ptr<Node> Parser::ContinueExpression(){ return nullptr; }
unique_ptr<Node> Parser::BreakExpression(){ return nullptr; }
unique_ptr<Node> Parser::IfExpression(){ return nullptr; }
unique_ptr<Node> Parser::ReturnExpression(){ return nullptr; }
unique_ptr<Node> Parser::Identifier(){ return nullptr; }
unique_ptr<Node> Parser::CHAR_STR_LITERAL(){ return nullptr; }
unique_ptr<Node> Parser::INTEGER_LITERAL(){ return nullptr; }
unique_ptr<Node> Parser::FLOAT_LITERAL(){ return nullptr; }
unique_ptr<Node> Parser::BOOLEAN_LITERAL(){ return nullptr; }
unique_ptr<Node> Parser::CycleExpression(){ return nullptr; }
unique_ptr<Node> Parser::WhileExpression(){ return nullptr; }
unique_ptr<Node> Parser::ForExpression(){ return nullptr; }
unique_ptr<Node> Parser::LoopExpression(){ return nullptr; }
unique_ptr<Node> Parser::FunctionIdentifier(){
	auto value = eat(token_type::IDENTIFIER);
	unique_ptr<Node> functionIdentifier(new Node(value, node_type::Identifier));
	return functionIdentifier;
}
unique_ptr<Node> Parser::ParameterList(){ return nullptr; }
unique_ptr<Node> Parser::CallParameterList(){ return nullptr; }
unique_ptr<Node> Parser::AssignmentOperator(){ return nullptr; }
unique_ptr<Node> Parser::LogicalAndExpression(){ return nullptr; }
unique_ptr<Node> Parser::InclusiveOrExpression(){ return nullptr; }
unique_ptr<Node> Parser::ExclusiveOrExpression(){ return nullptr; }
unique_ptr<Node> Parser::AndExpression(){ return nullptr; }
unique_ptr<Node> Parser::EqualityExpression(){ return nullptr; }
unique_ptr<Node> Parser::RelationalExpression(){ return nullptr; }
unique_ptr<Node> Parser::ShiftExpression(){ return nullptr; }
unique_ptr<Node> Parser::AdditiveExpression(){ return nullptr; }
unique_ptr<Node> Parser::MultiplicativeExpression(){ return nullptr; }
unique_ptr<Node> Parser::NotExpression(){ return nullptr; }
unique_ptr<Node> Parser::PrimaryExpression(){ return nullptr; }
unique_ptr<Node> Parser::PRINTLN(){ return nullptr; }

void Parser::error(const string message)
{
	cout << "PARSE ERROR!" << message << " ";
	cout << "Current token: " << lex->current_token()->get_lexeme() << endl;

	throw logic_error(message);
}
