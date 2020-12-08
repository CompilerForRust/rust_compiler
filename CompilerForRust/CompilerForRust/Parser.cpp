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

bool Parser::tryEat(token_type type, const int offset)
{
	return lex->tokens[lex->current_token_index + offset]->type == type;
}
unique_ptr<Node> Parser::Program(){
	auto functionDefinitionsChild = FunctionDefinitions();
	unique_ptr<Node> programNode(new Node("", node_type::Program));
	programNode->addChildNode(move(functionDefinitionsChild));
	return programNode;
}

unique_ptr<Node> Parser::FunctionDefinitions() {
	/*if (tryEat(token_type::FN))
	{
		auto functionDefinitionChild = FunctionDefinition();
		auto functionDefinitionsChild = FunctionDefinitions();
		unique_ptr<Node> functionDefinitionsNode(new Node("", node_type::FunctionDefinitions));
		functionDefinitionsNode->addChildNode(move(functionDefinitionChild));
		functionDefinitionsNode->addChildNode(move(functionDefinitionsChild));
		return functionDefinitionsNode;
	}
	else
		return nullptr;*/
	
	unique_ptr<Node> functionDefinitionsNode(new Node("", node_type::FunctionDefinitions));
	while (tryEat(token_type::FN)) {
		auto functionDefinitionChild = FunctionDefinition();
		functionDefinitionsNode->addChildNode(move(functionDefinitionChild));
	}
	return functionDefinitionsNode;
}

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

unique_ptr<Node> Parser::Statements() {
	unique_ptr<Node> statementsNode(new Node("", node_type::Statements));
	//if (tryEat(token_type::LET) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
	//	tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE) ||
	//	tryEat(token_type::LBRACE) || tryEat(token_type::NOT) || tryEat(token_type::IDENTIFIER) ||
	//	tryEat(token_type::LPAR) ||   tryEat(token_type::CONTINUE) || tryEat(token_type::BREAK) ||
	//	 tryEat(token_type::RETURN) || tryEat(token_type::TYPE))
	//{
	//	auto statementChlid = Statement();
	//	eat(token_type::SEMICOLON);
	//	auto statementsChild = Statements();
	//	statementsNode->addChildNode(move(statementChlid));
	//	statementsNode->addChildNode(move(statementsChild));
	//	return statementsNode;
	//}	
	//else if (tryEat(token_type::WHILE) || tryEat(token_type::FOR) || tryEat(token_type::LOOP)) {
	//	auto cycleExpressionChild = CycleExpression();
	//	statementsNode->addChildNode(move(cycleExpressionChild));
	//	auto statementsChild = Statements();
	//	statementsNode->addChildNode(move(statementsChild));
	//	return statementsNode;
	//}
	//else if (tryEat(token_type::IF)) {
	//	auto ifExpressionChild = IfExpression();
	//	statementsNode->addChildNode(move(ifExpressionChild));
	//	auto statementsChild = Statements();
	//	statementsNode->addChildNode(move(statementsChild));
	//	return statementsNode;
	//}
	//else
	//	return nullptr; 
	while (tryEat(token_type::LET) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE) ||
		tryEat(token_type::LBRACE) || tryEat(token_type::NOT) || tryEat(token_type::IDENTIFIER) ||
		tryEat(token_type::LPAR) || tryEat(token_type::CONTINUE) || tryEat(token_type::BREAK) ||
		tryEat(token_type::RETURN) || tryEat(token_type::TYPE) || tryEat(token_type::WHILE) ||
		tryEat(token_type::FOR) || tryEat(token_type::LOOP) || tryEat(token_type::IF)) {
		if (tryEat(token_type::LET) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
			tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE) ||
			tryEat(token_type::LBRACE) || tryEat(token_type::NOT) || tryEat(token_type::IDENTIFIER) ||
			tryEat(token_type::LPAR) || tryEat(token_type::CONTINUE) || tryEat(token_type::BREAK) ||
			tryEat(token_type::RETURN) || tryEat(token_type::TYPE))
		{
			auto statementChlid = Statement();
			eat(token_type::SEMICOLON);
			statementsNode->addChildNode(move(statementChlid));
		}
		else if (tryEat(token_type::WHILE) || tryEat(token_type::FOR) || tryEat(token_type::LOOP)) {
			auto cycleExpressionChild = CycleExpression();
			statementsNode->addChildNode(move(cycleExpressionChild));
		}
		else if (tryEat(token_type::IF)) {
			auto ifExpressionChild = IfExpression();
			statementsNode->addChildNode(move(ifExpressionChild));
		}
	}
	return statementsNode;
}
unique_ptr<Node> Parser::Statement(){
	if (tryEat(token_type::LET)) {
		auto declarationStatementChild = DeclarationStatement();
		unique_ptr<Node> statementNode(new Node("", node_type::Statement));
		statementNode->addChildNode(move(declarationStatementChild));
		return statementNode;
	}	
	else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE) ||
		tryEat(token_type::LBRACE) || tryEat(token_type::NOT) || tryEat(token_type::LPAR) ||
		tryEat(token_type::CONTINUE) || tryEat(token_type::BREAK) || tryEat(token_type::RETURN)) {
		auto expressionStatementChild = ExpressionStatement();
		unique_ptr<Node> statementNode(new Node("", node_type::Statement));
		statementNode->addChildNode(move(expressionStatementChild));
		return statementNode;
	}
	else if (tryEat(token_type::PRINTLN)) {
		auto printlnChild = PRINTLN();
		unique_ptr<Node> statementNode(new Node("", node_type::Statement));
		statementNode->addChildNode(move(printlnChild));
		return statementNode;
	}
	else if (tryEat(token_type::IDENTIFIER))
		if (tryEat(token_type::SET, 1) || tryEat(token_type::STAREQUAL, 1) || tryEat(token_type::SLASHEQUAL, 1) ||
			tryEat(token_type::PERCENTEQUAL, 1) || tryEat(token_type::PLUSEQUAL, 1) || tryEat(token_type::MINUSEQUAL, 1) ||
			tryEat(token_type::LSHIFTEQUAL, 1) || tryEat(token_type::RSHIFTEQUAL, 1) || tryEat(token_type::ANDEQUAL, 1) ||
			tryEat(token_type::OREQUAL, 1) || tryEat(token_type::ANDEQUAL, 1) || tryEat(token_type::XOREQUAL, 1)) {
			auto assignmentExpressionChild = AssignmentExpression();
			unique_ptr<Node> statementNode(new Node("", node_type::Statement));
			statementNode->addChildNode(move(assignmentExpressionChild));
			return statementNode;
		}
			
		else {
			auto expressionStatementChild = ExpressionStatement();
			unique_ptr<Node> statementNode(new Node("", node_type::Statement));
			statementNode->addChildNode(move(expressionStatementChild));
			return statementNode;
		}		
	//else if (tryEat(token_type::TYPE)) {
	else {
		auto typeExpressionChild = TypeExpression();
		unique_ptr<Node> statementNode(new Node("", node_type::Statement));
		statementNode->addChildNode(move(typeExpressionChild));
		return statementNode;
	}
}
unique_ptr<Node> Parser::DeclarationStatement(){
	eat(token_type::LET);
	unique_ptr<Node>declarationStatementNode(new Node("", node_type::DeclarationStatement));
	auto variableDefinitionChild = VariableDefinition();
	declarationStatementNode->addChildNode(move(variableDefinitionChild));
	if (tryEat(token_type::COLON)) {
		eat(token_type::COLON);
		auto dataTypeChild = DataType();
		declarationStatementNode->addChildNode(move(dataTypeChild));
		if (tryEat(token_type::SET)) {
			eat(token_type::SET);
			auto declarationRightStatementChild = DeclarationRightStatement();
			declarationStatementNode->addChildNode(move(declarationRightStatementChild));
		}
	}
	else if (tryEat(token_type::SET)) {
		eat(token_type::SET);
		auto declarationRightStatementChild = DeclarationRightStatement();
		declarationStatementNode->addChildNode(move(declarationRightStatementChild));
	}
	return declarationStatementNode;
}
unique_ptr<Node>Parser::DeclarationRightStatement() {
	unique_ptr<Node>declarationRightStatementNode(new Node("",node_type:: DeclarationRightStatement));
	if (tryEat(token_type::LBRACE)) {
		auto blockExpressionChild = BlockExpression();
		declarationRightStatementNode->addChildNode(move(blockExpressionChild));
	}
	else if (tryEat(token_type::LPAR)) {
		auto groupedExpressionChild = GroupedExpression();
		declarationRightStatementNode->addChildNode(move(groupedExpressionChild));
	}
	else if (tryEat(token_type::NOT)|| tryEat(token_type::CHARACTER)|| tryEat(token_type::NUMBER)
		|| tryEat(token_type::DOUBLE_NUMBER)|| tryEat(token_type::TRUE)|| tryEat(token_type::FALSE)) {
		auto logicalOrExpressionChild = LogicalOrExpression();
		declarationRightStatementNode->addChildNode(move(logicalOrExpressionChild));
	}
	else if (tryEat(token_type::IDENTIFIER) && tryEat(token_type::LPAR,1)) {
		auto functionCallChild = FunctionCall();
		declarationRightStatementNode->addChildNode(move(functionCallChild));
	}
	else {
		auto logicalOrExpressionChild = LogicalOrExpression();
		declarationRightStatementNode->addChildNode(move(logicalOrExpressionChild));
	}
	return declarationRightStatementNode;
}
unique_ptr<Node> Parser::ExpressionStatement(){ 
	unique_ptr<Node> child;
	if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE))
		child = LiteralExpression();
	else if (tryEat(token_type::LBRACE))
		child = BlockExpression();
	else if (tryEat(token_type::NOT))
		child = LogicalOrExpression();
	else if (tryEat(token_type::LPAR))
		child = GroupedExpression();
	else if (tryEat(token_type::IDENTIFIER))
		child = FunctionCall();
	else if (tryEat(token_type::CONTINUE))
		child = ContinueExpression();
	else if (tryEat(token_type::BREAK))
		child = BreakExpression();
	//else if (tryEat(token_type::RETURN))
	else
		child = ReturnExpression();
	unique_ptr<Node> expressionStatementNode(new Node("", node_type::ExpressionStatement));
	expressionStatementNode->addChildNode(move(child));
	return expressionStatementNode;
}
unique_ptr<Node> Parser::AssignmentExpression(){
		auto variableChild = Variable();
		auto assignmentOperatorChild = AssignmentOperator();
		auto logicalOrExpressionChild = LogicalOrExpression();
		unique_ptr<Node> assignmentExpressionNode(new Node("", node_type::AssignmentExpression));
		assignmentExpressionNode->addChildNode(move(variableChild));
		assignmentExpressionNode->addChildNode(move(assignmentOperatorChild));
		assignmentExpressionNode->addChildNode(move(logicalOrExpressionChild));
		return assignmentExpressionNode;
}
unique_ptr<Node> Parser::TypeExpression(){ 
	eat(token_type::TYPE);
	auto identifierChild1 = Identifier();
	eat(token_type::SET);
	auto identifierChild2 = Identifier();
	unique_ptr<Node> typeExpressionNode(new Node("", node_type::TypeExpression));
	typeExpressionNode->addChildNode(move(identifierChild1));
	typeExpressionNode->addChildNode(move(identifierChild2));
	return typeExpressionNode;
}
unique_ptr<Node> Parser::VariableDefinition(){
	unique_ptr<Node>variableDefinitionNode(new Node("", node_type::VariableDefinition));
	if (tryEat(token_type::MUT)) {
		eat(token_type::MUT);
		auto variableChild = Variable();
		variableDefinitionNode->addChildNode(move(variableChild));

	}
	else {
		auto variableChild = Variable();
		variableDefinitionNode->addChildNode(move(variableChild));
	}
	return variableDefinitionNode;
}
unique_ptr<Node> Parser::DataType(){
	string value;
	if (tryEat(token_type::I16))
		value = eat(token_type::I16);
	else if (tryEat(token_type::U16))
		value = eat(token_type::U16);
	else if (tryEat(token_type::BOOL))
		value = eat(token_type::BOOL);
	else if (tryEat(token_type::F32))
		value = eat(token_type::F32);
	else
		value = eat(token_type::CHAR);
	unique_ptr<Node> dataTypeNode(new Node(value, node_type::DataType));
	return dataTypeNode;
}
unique_ptr<Node> Parser::LiteralExpression(){
	unique_ptr<Node>literalExpressionNode(new Node("", node_type::LiteralExpression));
	if (tryEat(token_type::CHARACTER)) {
		auto charStrLiteralChild = CHAR_STR_LITERAL();
		literalExpressionNode->addChildNode(move(charStrLiteralChild));
	}
	else if (tryEat(token_type::NUMBER)) {
		auto integerLiteralChild = INTEGER_LITERAL();
		literalExpressionNode->addChildNode(move(integerLiteralChild));
	}
	else if (tryEat(token_type::DOUBLE_NUMBER)) {
		auto floatLiteralChild = FLOAT_LITERAL();
		literalExpressionNode->addChildNode(move(floatLiteralChild));
	}
	else {
		auto booleanLiteralChild = BOOLEAN_LITERAL();
		literalExpressionNode->addChildNode(move(booleanLiteralChild));
	}
	return literalExpressionNode;
}
unique_ptr<Node> Parser::Variable(){
	auto value = eat(token_type::IDENTIFIER);
	unique_ptr<Node> variableNode(new Node(value, node_type::Variable));
	return variableNode;
}

unique_ptr<Node> Parser::BlockExpression(){
	unique_ptr<Node> blockExpressionNode(new Node("", node_type::BlockExpression));
	unique_ptr<Node>child;
    eat(token_type::LBRACE);
	child = Statements();
	eat(token_type::RBRACE);
	blockExpressionNode->addChildNode(move(child));
	return blockExpressionNode;
}

unique_ptr<Node> Parser::LogicalOrExpression(){
	unique_ptr<Node>logicalOrExpressionNode(new Node("", node_type::LogicalOrExpression));
	auto logicalAndExpressionChild = LogicalAndExpression();
	auto logicalOrExpressionEChild = LogicalOrExpressionE();
	logicalOrExpressionNode->addChildNode(move(logicalAndExpressionChild));
	logicalOrExpressionNode->addChildNode(move(logicalOrExpressionEChild));
	return logicalOrExpressionNode;
}
unique_ptr<Node> Parser::GroupedExpression(){
	eat(token_type::LPAR);
	auto expressionStatementChild = ExpressionStatement();
	eat(token_type::RPAR);
	unique_ptr<Node> groupedExpressionNode(new Node("", node_type::BlockExpression));
	groupedExpressionNode->addChildNode(move(expressionStatementChild));
	return groupedExpressionNode;
}
unique_ptr<Node> Parser::FunctionCall() {
	auto functionIdentifierChild = FunctionIdentifier();
	eat(token_type::LPAR);
	auto callParameterListChild = CallParameterList();
	eat(token_type::RPAR);
	unique_ptr<Node> functionCallNode(new Node("", node_type::FunctionCall));
	functionCallNode->addChildNode(move(functionIdentifierChild));
	functionCallNode->addChildNode(move(callParameterListChild));
	return functionCallNode;
}
unique_ptr<Node> Parser::ContinueExpression(){ 
	eat(token_type::CONTINUE);
	unique_ptr<Node> continueExpressionNode(new Node("continue", node_type::ContinueExpression));
	return continueExpressionNode;
}
unique_ptr<Node> Parser::BreakExpression(){ 
	eat(token_type::BREAK);
	unique_ptr<Node> breakExpressionNode(new Node("break", node_type::BreakExpression));
	return breakExpressionNode;
}
unique_ptr<Node> Parser::IfExpression(){
	unique_ptr<Node> ifExpressionNode(new Node("", node_type::IfExpression));
	eat(token_type::IF);
	auto conditionStatementChild = ConditionStatement();
	ifExpressionNode->addChildNode(move(conditionStatementChild));
	auto blockExpressionChild1 = BlockExpression();
	ifExpressionNode->addChildNode(move(blockExpressionChild1));
	if (tryEat(token_type::ELSE)) {
		eat(token_type::ELSE);
		if (tryEat(token_type::IF)) {
			auto ifExpressionChild = IfExpression();
			ifExpressionNode->addChildNode(move(conditionStatementChild));
			eat(token_type::ELSE);
			auto blockExpressionChild2 = BlockExpression();
			ifExpressionNode->addChildNode(move(blockExpressionChild2));
		}
		else {
			auto blockExpressionChild3 = BlockExpression();
			ifExpressionNode->addChildNode(move(blockExpressionChild3));
		}
	}
	return ifExpressionNode;
}
unique_ptr<Node> Parser::ReturnExpression(){ 
	eat(token_type::RETURN);
	auto expressionStatementChild = ExpressionStatement();
	unique_ptr<Node> returnExpressionNode(new Node("", node_type::BlockExpression));
	returnExpressionNode->addChildNode(move(expressionStatementChild));
	return returnExpressionNode;
}
unique_ptr<Node> Parser::Identifier(){ 
	auto value = eat(token_type::IDENTIFIER);
	unique_ptr<Node> identifierNode(new Node(value, node_type::Identifier));
	return identifierNode;
}
unique_ptr<Node> Parser::CHAR_STR_LITERAL(){
	string value = eat(token_type::CHARACTER);
	unique_ptr<Node>charStrLiteralNode(new Node(value, node_type::CHAR_STR_LITERAL));
	return charStrLiteralNode;
}
unique_ptr<Node> Parser::INTEGER_LITERAL(){
	string value = eat(token_type::NUMBER);
	unique_ptr<Node>integerLiteralNode(new Node(value, node_type::INTEGER_LITERAL));
	return integerLiteralNode;
}
unique_ptr<Node> Parser::FLOAT_LITERAL(){
	string value = eat(token_type::DOUBLE_NUMBER);
	unique_ptr<Node>floatLiteralNode(new Node(value, node_type::FLOAT_LITERAL));
	return floatLiteralNode;
}
unique_ptr<Node> Parser::BOOLEAN_LITERAL(){
	string value;
	if (tryEat(token_type::TRUE)) {
		value = eat(token_type::TRUE);
	}
	else {
		value = eat(token_type::FALSE);
	}
	unique_ptr<Node>booleanLiteralNode(new Node(value, node_type::BOOLEAN_LITERAL));
	return booleanLiteralNode;
}
unique_ptr<Node> Parser::CycleExpression(){
	unique_ptr<Node> child;
	if (tryEat(token_type::WHILE)) {
		child = WhileExpression();
	}
	else if (tryEat(token_type::FOR)) {
		child = ForExpression();
	}
	//else if (tryEat(token_type::LOOP)) {
	else {
		child = LoopExpression();
	}
	unique_ptr<Node> cycleExpressionNode(new Node("", node_type::CycleExpression));
	cycleExpressionNode->addChildNode(move(child));
	return cycleExpressionNode;
}
unique_ptr<Node> Parser::WhileExpression(){
	eat(token_type::WHILE);
	auto expressionStatementChild = ExpressionStatement();
	auto blockExpressionChild = BlockExpression();
	unique_ptr<Node> whileExpressionNode(new Node("", node_type::WhileExpression));
	whileExpressionNode->addChildNode(move(expressionStatementChild));
	whileExpressionNode->addChildNode(move(blockExpressionChild));
	return whileExpressionNode;
}
unique_ptr<Node> Parser::ForExpression(){
	eat(token_type::FOR);
	auto identifierChild = Identifier();
	eat(token_type::IN);
	auto shiftExpressionChild1 = ShiftExpression();
	eat(token_type::DOUBLE_POINT);
	auto shiftExpressionChild2 = ShiftExpression();
	auto blockExpressionChild = BlockExpression();
	unique_ptr<Node> forExpressionNode(new Node("", node_type::ForExpression));
	forExpressionNode->addChildNode(move(identifierChild));
	forExpressionNode->addChildNode(move(shiftExpressionChild1));
	forExpressionNode->addChildNode(move(shiftExpressionChild2));
	forExpressionNode->addChildNode(move(blockExpressionChild));
	return forExpressionNode;
}
unique_ptr<Node> Parser::LoopExpression() {
	eat(token_type::LOOP);
	auto blockExpressionChild = BlockExpression();
	unique_ptr<Node> loopExpressionNode(new Node("", node_type::LoopExpression));
	loopExpressionNode->addChildNode(move(blockExpressionChild));
	return loopExpressionNode;
}
unique_ptr<Node> Parser::FunctionIdentifier(){
	auto value = eat(token_type::IDENTIFIER);
	unique_ptr<Node> functionIdentifier(new Node(value, node_type::Identifier));
	return functionIdentifier;
}
unique_ptr<Node> Parser::ParameterList(){
	unique_ptr<Node>parameterListNode(new Node("", node_type::ParameterList));
	if (tryEat(token_type::IDENTIFIER)) {
		auto variableChild = Variable();
		parameterListNode->addChildNode(move(variableChild));
		eat(token_type::COLON);
		auto dataTypeNode = DataType();
		parameterListNode->addChildNode(move(dataTypeNode));
		eat(token_type::COMMA);
		auto parameterListChild = ParameterList();
		parameterListNode->addChildNode(move(parameterListChild));
		return parameterListNode;
	}
	return nullptr;
}
unique_ptr<Node> Parser::CallParameterList(){
	unique_ptr<Node>callParameterListNode(new Node("", node_type::CallParameterList));
	if (tryEat(token_type::IDENTIFIER)) {
		auto variableChild = Variable();
		callParameterListNode->addChildNode(move(variableChild));
		eat(token_type::COMMA);
		auto callParameterListChild = CallParameterList();
		callParameterListNode->addChildNode(move(callParameterListChild));
		return callParameterListNode;
	}
	else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL)) {
		auto literalExpressionChild = LiteralExpression();
		callParameterListNode->addChildNode(move(literalExpressionChild));
		eat(token_type::COMMA);
		auto callParameterListChild = CallParameterList();
		callParameterListNode->addChildNode(move(callParameterListChild));
		return callParameterListNode;
	}
	return nullptr;
}
unique_ptr<Node> Parser::AssignmentOperator(){
	string value;
	if (tryEat(token_type::SET))
		value = eat(token_type::SET);
	else if (tryEat(token_type::STAREQUAL))
		value = eat(token_type::STAREQUAL);
	else if (tryEat(token_type::PERCENTEQUAL))
		value = eat(token_type::PERCENTEQUAL);
	else if (tryEat(token_type::PLUSEQUAL))
		value = eat(token_type::PLUSEQUAL);
	else if (tryEat(token_type::MINUSEQUAL))
		value = eat(token_type::MINUSEQUAL);
	else if (tryEat(token_type::RSHIFTEQUAL))
		value = eat(token_type::RSHIFTEQUAL);
	else if (tryEat(token_type::LSHIFTEQUAL))
		value = eat(token_type::LSHIFTEQUAL);
	else if (tryEat(token_type::XOREQUAL))
		value = eat(token_type::XOREQUAL);
	else if (tryEat(token_type::ANDEQUAL))
		value = eat(token_type::ANDEQUAL);
	else if (tryEat(token_type::OREQUAL))
		value = eat(token_type::OREQUAL);
	//else if (tryEat(token_type::SLASHEQUAL))
	else
		value = eat(token_type::SLASHEQUAL);
	unique_ptr<Node> assignmentOperatorNode(new Node(value, node_type::AssignmentOperator));
	return assignmentOperatorNode;
}
unique_ptr<Node> Parser::LogicalAndExpression(){
	unique_ptr<Node>logicalAndExpressionNode(new Node("", node_type::LogicalAndExpression));
	auto inclusiveOrExpressionChild = InclusiveOrExpression();
	auto logicalAndExpressionEChild = LogicalAndExpressionE();
	logicalAndExpressionNode->addChildNode(move(inclusiveOrExpressionChild));
	logicalAndExpressionNode->addChildNode(move(logicalAndExpressionEChild));
	return logicalAndExpressionNode;
}
unique_ptr<Node> Parser::InclusiveOrExpression(){ 
	unique_ptr<Node>inclusiveOrExpressionNode(new Node("", node_type::InclusiveOrExpression));
	auto exclusiveOrExpressionChild = ExclusiveOrExpression();
	auto inclusiveOrExpressionEChild = InclusiveOrExpressionE();
	inclusiveOrExpressionNode->addChildNode(move(exclusiveOrExpressionChild));
	inclusiveOrExpressionNode->addChildNode(move(inclusiveOrExpressionEChild));
	return inclusiveOrExpressionNode;
}
unique_ptr<Node> Parser::ExclusiveOrExpression() {
	unique_ptr<Node>exclusiveOrExpressionNode(new Node("", node_type::ExclusiveOrExpression));
	auto andExpressionChild = AndExpression();
	auto exclusiveOrExpressionEChild = ExclusiveOrExpressionE();
	exclusiveOrExpressionNode->addChildNode(move(andExpressionChild));
	exclusiveOrExpressionNode->addChildNode(move(exclusiveOrExpressionEChild));
	return exclusiveOrExpressionNode;
}
unique_ptr<Node> Parser::AndExpression() {
	unique_ptr<Node>andExpressionNode(new Node("", node_type::AndExpression));
	auto equalityExpressionChild = EqualityExpression();
	auto andExpressionEChild = AndExpressionE();
	andExpressionNode->addChildNode(move(equalityExpressionChild));
	andExpressionNode->addChildNode(move(andExpressionEChild));
	return andExpressionNode;
}
unique_ptr<Node> Parser::EqualityExpression() {
	unique_ptr<Node>equalityExpressionNode(new Node("", node_type::EqualityExpression));
	auto relationalExpressionChild = RelationalExpression();
	auto equalityExpressionENode = EqualityExpressionE();
	equalityExpressionNode->addChildNode(move(relationalExpressionChild));
	equalityExpressionNode->addChildNode(move(equalityExpressionENode));
	return equalityExpressionNode;
}
unique_ptr<Node> Parser::RelationalExpression() {
	unique_ptr<Node>relationalExpressionNode(new Node("", node_type::RelationalExpression));
	auto shiftExpressionChild = ShiftExpression();
	auto relationalExpressionEChild = RelationalExpressionE();
	relationalExpressionNode->addChildNode(move(shiftExpressionChild));
	relationalExpressionNode->addChildNode(move(relationalExpressionEChild));
	return relationalExpressionNode;
}
unique_ptr<Node> Parser::ShiftExpression() {
	unique_ptr<Node>shiftExpressionNode(new Node("", node_type::ShiftExpression));
	auto additiveExpressionChild = AdditiveExpression();
	auto shiftExpressionEChild = ShiftExpressionE();
	shiftExpressionNode->addChildNode(move(additiveExpressionChild));
	shiftExpressionNode->addChildNode(move(shiftExpressionEChild));
	return shiftExpressionNode;
}
unique_ptr<Node> Parser::AdditiveExpression() {
	unique_ptr<Node>additiveExpressionNode(new Node("", node_type::AdditiveExpression));
	auto multiplicativeExpressionChild = MultiplicativeExpression();
	auto additiveExpressionEChild = AdditiveExpressionE();
	additiveExpressionNode->addChildNode(move(multiplicativeExpressionChild));
	additiveExpressionNode->addChildNode(move(additiveExpressionEChild));
	return additiveExpressionNode;
}
unique_ptr<Node> Parser::MultiplicativeExpression(){
	unique_ptr<Node>multiplicativeExpressionNode(new Node("", node_type::MultiplicativeExpression));
	auto notExpressionChild = NotExpression();
	auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
	multiplicativeExpressionNode->addChildNode(move(notExpressionChild));
	multiplicativeExpressionNode->addChildNode(move(multiplicativeExpressionEChild));
	return multiplicativeExpressionNode;
}
unique_ptr<Node> Parser::NotExpression(){
	unique_ptr<Node> child;
	if (tryEat(token_type::NOT)) {
		eat(token_type::NOT);
		child = PrimaryExpression();
	}
	//else if (tryEat(token_type::IDENTIFIER) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
	//	tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL) || tryEat(token_type::LPAR))
	else
		child = PrimaryExpression();
	unique_ptr<Node>notExpressionENode(new Node("", node_type::NotExpression));
	notExpressionENode->addChildNode(move(child));
	return notExpressionENode;
}
unique_ptr<Node> Parser::PrimaryExpression(){
	unique_ptr<Node> child;
	if (tryEat(token_type::IDENTIFIER))
		child = Variable();
	else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL))
		child = LiteralExpression();
	//else if (tryEat(token_type::LPAR)) {
	else {
		eat(token_type::LPAR);
		child = LogicalOrExpression();
		eat(token_type::RPAR);
	}
	unique_ptr<Node>primaryExpressionENode(new Node("", node_type::PrimaryExpression));
	primaryExpressionENode->addChildNode(move(child));
	return primaryExpressionENode;
}
//Ìõ¼þÓï¾ä
unique_ptr<Node>Parser::ConditionStatement() {
	unique_ptr<Node>conditionStatementNode(new Node("", node_type::ConditionStatement));
	if (tryEat(token_type::LBRACE)) {
		auto blockExpressionChild = BlockExpression();
		conditionStatementNode->addChildNode(move(blockExpressionChild));
	}
	else if (tryEat(token_type::LPAR)) {
		auto groupedExpressionChild = GroupedExpression();
		conditionStatementNode->addChildNode(move(groupedExpressionChild));
	}
	else if (tryEat(token_type::IF)) {
		auto ifExpressionChild = IfExpression();
		conditionStatementNode->addChildNode(move(ifExpressionChild));
	}
	else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER)
		|| tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE)
		|| tryEat(token_type::FALSE) || tryEat(token_type::NOT)) {
		auto logicalOrExpressionChild = LogicalOrExpression();
		conditionStatementNode->addChildNode(move(logicalOrExpressionChild));
	}
	else {
		if (tryEat(token_type::IDENTIFIER)&&tryEat(token_type::LPAR, 1)) {
			auto functionCallChild = FunctionCall();
			conditionStatementNode->addChildNode(move(functionCallChild));
		}
		else
		{
			auto logicalOrExpressionChild = LogicalOrExpression();
			conditionStatementNode->addChildNode(move(logicalOrExpressionChild));
		}
	}
	return conditionStatementNode;
}
unique_ptr<Node> Parser::PRINTLN(){ 
	unique_ptr<Node>printlnNode(new Node("", node_type::PRINTLN));
	eat(token_type::PRINTLN);
	eat(token_type::LPAR);
	eat(token_type::QUOTES);
	while (!tryEat(token_type::QUOTES))
	{
		lex->next_token();
	}
	eat(token_type::QUOTES);
	eat(token_type::COMMA);
	auto callParameterListChild = CallParameterList();
	printlnNode->addChildNode(move(callParameterListChild));
	eat(token_type::LPAR);
	return printlnNode;
}

//Ïû³ý×óµÝ¹é
unique_ptr<Node>Parser::LogicalOrExpressionE() {
	unique_ptr<Node>logicalOrExpressionENode(new Node("", node_type::LogicalOrExpressionE));

	if (tryEat(token_type::LOGICOR)) {
		eat(token_type::LOGICOR);
		auto logicalAndExpressionChild = LogicalAndExpression();
		auto logicalOrExpressionEChild = LogicalOrExpressionE();
		logicalOrExpressionENode->addChildNode(move(logicalAndExpressionChild));
		logicalOrExpressionENode->addChildNode(move(logicalOrExpressionEChild));
		return logicalOrExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::LogicalAndExpressionE() {
	unique_ptr<Node>logicalAndExpressionENode(new Node("", node_type::LogicalAndExpressionE));

	if (tryEat(token_type::LOGICAND)) {
		eat(token_type::LOGICAND);
		auto inclusiveOrExpressionChild = InclusiveOrExpression();
		auto logicalAndExpressionEChild = LogicalAndExpressionE();
		logicalAndExpressionENode->addChildNode(move(inclusiveOrExpressionChild));
		logicalAndExpressionENode->addChildNode(move(logicalAndExpressionEChild));
		return logicalAndExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::InclusiveOrExpressionE() {
	unique_ptr<Node>inclusiveOrExpressionENode(new Node("", node_type::InclusiveOrExpressionE));
	if (tryEat(token_type::OR)) {
		eat(token_type::OR);
		auto exclusiveOrExpressionChild = ExclusiveOrExpression();
		auto inclusiveOrExpressionEChild = InclusiveOrExpressionE();
		inclusiveOrExpressionENode->addChildNode(move(exclusiveOrExpressionChild));
		inclusiveOrExpressionENode->addChildNode(move(inclusiveOrExpressionEChild));
		return inclusiveOrExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::ExclusiveOrExpressionE() {
	unique_ptr<Node>exclusiveOrExpressionENode(new Node("", node_type::ExclusiveOrExpressionE));
	if (tryEat(token_type::XOR)) {
		eat(token_type::XOR);
		auto andExpressionChild = AndExpression();
		auto exclusiveOrExpressionEChild = ExclusiveOrExpressionE();
		exclusiveOrExpressionENode->addChildNode(move(andExpressionChild));
		exclusiveOrExpressionENode->addChildNode(move(exclusiveOrExpressionEChild));
		return exclusiveOrExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::AndExpressionE() {
	unique_ptr<Node>andExpressionENode(new Node("", node_type::AndExpressionE));
	if (tryEat(token_type::AND)) {
		eat(token_type::AND);
		auto equalityExpressionChild = EqualityExpression();
		auto andExpressionEChild = AndExpressionE();
		andExpressionENode->addChildNode(move(equalityExpressionChild));
		andExpressionENode->addChildNode(move(andExpressionEChild));
		return andExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::EqualityExpressionE() {
	unique_ptr<Node>equalityExpressionENode(new Node("", node_type::EqualityExpressionE));
	if (tryEat(token_type::EQUALITY)) {
		eat(token_type::EQUALITY);
		auto relationalExpressionChild = RelationalExpression();
		auto equalityExpressionEChild = EqualityExpressionE();
		equalityExpressionENode->addChildNode(move(relationalExpressionChild));
		equalityExpressionENode->addChildNode(move(equalityExpressionEChild));
		return equalityExpressionENode;
	}
	else if (tryEat(token_type::NOTEQUAL)) {
		eat(token_type::NOTEQUAL);
		auto relationalExpressionChild = RelationalExpression();
		auto equalityExpressionEChild = EqualityExpressionE();
		equalityExpressionENode->addChildNode(move(relationalExpressionChild));
		equalityExpressionENode->addChildNode(move(equalityExpressionEChild));
		return equalityExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::RelationalExpressionE() {
	unique_ptr<Node>relationalExpressionENode(new Node("", node_type::RelationalExpressionE));
	if (tryEat(token_type::LESS)) {
		eat(token_type::LESS);
		auto shiftExpressionChild = ShiftExpression();
		auto relationalExpressionEChild = RelationalExpressionE();
		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
		return relationalExpressionENode;
	}
	else if (tryEat(token_type::MORE)) {
		eat(token_type::MORE);
		auto shiftExpressionChild = ShiftExpression();
		auto relationalExpressionEChild = RelationalExpressionE();
		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
		return relationalExpressionENode;
	}
	else if (tryEat(token_type::LESSEQUAL)) {
		eat(token_type::LESSEQUAL);
		auto shiftExpressionChild = ShiftExpression();
		auto relationalExpressionEChild = RelationalExpressionE();
		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
		return relationalExpressionENode;
	}
	else if (tryEat(token_type::MOREEQUAL)) {
		eat(token_type::MOREEQUAL);
		auto shiftExpressionChild = ShiftExpression();
		auto relationalExpressionEChild = RelationalExpressionE();
		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
		return relationalExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::ShiftExpressionE() {
	unique_ptr<Node>shiftExpressionENode(new Node("", node_type::ShiftExpressionE));
	if (tryEat(token_type::LSHIFT)) {
		eat(token_type::LSHIFT);
		auto additiveExpressionChild = AdditiveExpression();
		auto shiftExpressionEChild = ShiftExpressionE();
		shiftExpressionENode->addChildNode(move(additiveExpressionChild));
		shiftExpressionENode->addChildNode(move(shiftExpressionEChild));
		return shiftExpressionENode;
	}
	else if (tryEat(token_type::RSHIFT)) {
		eat(token_type::RSHIFT);
		auto additiveExpressionChild = AdditiveExpression();
		auto shiftExpressionEChild = ShiftExpressionE();
		shiftExpressionENode->addChildNode(move(additiveExpressionChild));
		shiftExpressionENode->addChildNode(move(shiftExpressionEChild));
		return shiftExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::AdditiveExpressionE() {
	unique_ptr<Node>additiveExpressionENode(new Node("", node_type::AdditiveExpressionE));
	if (tryEat(token_type::PLUS)) {
		eat(token_type::PLUS);
		auto multiplicativeExpressionChild = MultiplicativeExpression();
		auto additiveExpressionEChild = AdditiveExpressionE();
		additiveExpressionENode->addChildNode(move(multiplicativeExpressionChild));
		additiveExpressionENode->addChildNode(move(additiveExpressionEChild));
		return additiveExpressionENode;
	}
	else if (tryEat(token_type::MINUS)) {
		eat(token_type::MINUS);
		auto multiplicativeExpressionChild = MultiplicativeExpression();
		auto additiveExpressionEChild = AdditiveExpressionE();
		additiveExpressionENode->addChildNode(move(multiplicativeExpressionChild));
		additiveExpressionENode->addChildNode(move(additiveExpressionEChild));
		return additiveExpressionENode;
	}
	else return nullptr;
}
unique_ptr<Node>Parser::MultiplicativeExpressionE() {
	unique_ptr<Node>multiplicativeExpressionENode(new Node("", node_type::MultiplicativeExpressionE));
	if (tryEat(token_type::STAR)) {
		eat(token_type::STAR);
		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else if (tryEat(token_type::SLASH)) {
		eat(token_type::SLASH);
		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else if (tryEat(token_type::PERCENT)) {
		eat(token_type::PERCENT);
		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else return nullptr;
}

void Parser::error(const string message)
{
	cout << "PARSE ERROR!" << message << " ";
	cout << "Current token: " << lex->current_token()->get_lexeme() << endl;

	throw logic_error(message);
}
