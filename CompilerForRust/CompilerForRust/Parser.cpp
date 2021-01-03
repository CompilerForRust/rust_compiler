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

string Parser::nowToken() {
	return lex->current_token_value();
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
	unique_ptr<Node> functionDefinitionNode(new Node("", node_type::FunctionDefinition));

	unique_ptr<Node> tokenNodeFN(new Node(eat(token_type::FN), node_type::Token));
	functionDefinitionNode->addChildNode(move(tokenNodeFN));

	auto functionIdentifierChild = FunctionIdentifier();
	functionDefinitionNode->addChildNode(move(functionIdentifierChild));

	unique_ptr<Node> tokenNodeLPAR(new Node(eat(token_type::LPAR), node_type::Token));
	functionDefinitionNode->addChildNode(move(tokenNodeLPAR));

	auto parameterListChild = ParameterList();
	functionDefinitionNode->addChildNode(move(parameterListChild));

	eat(token_type::RPAR);
	if (tryEat(token_type::RETURNTYPE)) {
		unique_ptr<Node> tokenNodeRETURNTYPE(new Node(eat(token_type::RETURNTYPE), node_type::Token));
		functionDefinitionNode->addChildNode(move(tokenNodeRETURNTYPE));

		auto dataTypeChild = DataType();
		functionDefinitionNode->addChildNode(move(dataTypeChild));

		auto blockExpressionChlid = BlockExpression();
		functionDefinitionNode->addChildNode(move(blockExpressionChlid));

		return functionDefinitionNode;
	}
	else {
		auto blockExpressionChild = BlockExpression();
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
		tryEat(token_type::FOR) || tryEat(token_type::LOOP) || tryEat(token_type::IF)||tryEat(token_type::PRINTLN)||tryEat(token_type::COMMENT)) {
		if (tryEat(token_type::LET) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
			tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::TRUE) || tryEat(token_type::FALSE) ||
			tryEat(token_type::LBRACE) || tryEat(token_type::NOT) || tryEat(token_type::IDENTIFIER) ||
			tryEat(token_type::LPAR) || tryEat(token_type::CONTINUE) || tryEat(token_type::BREAK) ||
			tryEat(token_type::RETURN) || tryEat(token_type::TYPE) || tryEat(token_type::PRINTLN))
		{
			auto statementChlid = Statement();
			statementsNode->addChildNode(move(statementChlid));

			unique_ptr<Node> tokenNodeSEMICOLON(new Node(eat(token_type::SEMICOLON), node_type::Token));
			statementsNode->addChildNode(move(tokenNodeSEMICOLON));

		}
		else if (tryEat(token_type::COMMENT)) {
			auto commentChild = COMMENT();
			statementsNode->addChildNode(move(commentChild));
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
	else if (tryEat(token_type::COMMENT)) {
		auto commentChild = COMMENT();
		unique_ptr<Node> statementNode(new Node("", node_type::Statement));
		statementNode->addChildNode(move(commentChild));
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
	unique_ptr<Node>declarationStatementNode(new Node("", node_type::DeclarationStatement));

	unique_ptr<Node>tokenNodeCOLON(new Node(eat(token_type::LET), node_type::Token));
	declarationStatementNode->addChildNode(move(tokenNodeCOLON));

	auto variableDefinitionChild = VariableDefinition();
	declarationStatementNode->addChildNode(move(variableDefinitionChild));

	if (tryEat(token_type::COLON)) {
		unique_ptr<Node>tokenNodeCOLON(new Node(eat(token_type::COLON), node_type::Token));
		declarationStatementNode->addChildNode(move(tokenNodeCOLON));

		auto dataTypeChild = DataType();
		declarationStatementNode->addChildNode(move(dataTypeChild));
		if (tryEat(token_type::SET)) {
			unique_ptr<Node>tokenNodeSET(new Node(eat(token_type::SET), node_type::Token));
			declarationStatementNode->addChildNode(move(tokenNodeSET));

			auto declarationRightStatementChild = DeclarationRightStatement();
			declarationStatementNode->addChildNode(move(declarationRightStatementChild));
		}
	}
	else if (tryEat(token_type::SET)) {
		unique_ptr<Node>tokenNodeSET(new Node(eat(token_type::SET), node_type::Token));
		declarationStatementNode->addChildNode(move(tokenNodeSET));

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
		auto BinaryExpressionChild = BinaryExpression();
		declarationRightStatementNode->addChildNode(move(BinaryExpressionChild));
	}
	else if (tryEat(token_type::IDENTIFIER) && tryEat(token_type::LPAR,1)) {
		auto functionCallChild = FunctionCall();
		declarationRightStatementNode->addChildNode(move(functionCallChild));
	}
	else {
		auto BinaryExpressionChild = BinaryExpression();
		declarationRightStatementNode->addChildNode(move(BinaryExpressionChild));
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
		child = BinaryExpression();
	else if (tryEat(token_type::LPAR))
		child = GroupedExpression();
	else if (tryEat(token_type::IDENTIFIER))
		if (tryEat(token_type::LPAR, 1))
			child = FunctionCall();
		else child = BinaryExpression();
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
		auto BinaryExpressionChild = BinaryExpression();
		unique_ptr<Node> assignmentExpressionNode(new Node("", node_type::AssignmentExpression));
		assignmentExpressionNode->addChildNode(move(variableChild));
		assignmentExpressionNode->addChildNode(move(assignmentOperatorChild));
		assignmentExpressionNode->addChildNode(move(BinaryExpressionChild));
		return assignmentExpressionNode;
}
unique_ptr<Node> Parser::TypeExpression(){ 
	unique_ptr<Node> typeExpressionNode(new Node("", node_type::TypeExpression));

	unique_ptr<Node> tokenNodeTYPE(new Node(eat(token_type::TYPE), node_type::Token));
	typeExpressionNode->addChildNode(move(tokenNodeTYPE));

	auto identifierChild1 = Identifier();
	typeExpressionNode->addChildNode(move(identifierChild1));


	unique_ptr<Node> tokenNodeSET(new Node(eat(token_type::SET), node_type::Token));
	typeExpressionNode->addChildNode(move(tokenNodeSET));

	auto identifierChild2 = Identifier();
	typeExpressionNode->addChildNode(move(identifierChild2));

	return typeExpressionNode;
}
unique_ptr<Node> Parser::VariableDefinition(){
	unique_ptr<Node>variableDefinitionNode(new Node("", node_type::VariableDefinition));
	if (tryEat(token_type::MUT)) {
		unique_ptr<Node>tokenNodeMUT(new Node(eat(token_type::MUT), node_type::Token));
		variableDefinitionNode->addChildNode(move(tokenNodeMUT));

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
	cout << nowToken();
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
	string value = nowToken();
	unique_ptr<Node> tokenNode(new Node(eat(token_type::LBRACE), node_type::Token));
	blockExpressionNode->addChildNode(move(tokenNode));

	child = Statements();
	blockExpressionNode->addChildNode(move(child));

	unique_ptr<Node> tokenNode1(new Node(eat(token_type::RBRACE), node_type::Token));
	blockExpressionNode->addChildNode(move(tokenNode1));
	
	return blockExpressionNode;
}
int Parser::GetTokPrecedence(string op) {
	auto key = BinopPrecedence.find(op);
	if (key != BinopPrecedence.end()) {
		return key->second;
	}
	else return -1;
}
unique_ptr<Node> Parser::ParseBinOpRHS(int ExprPrec,unique_ptr<Node> LHSChild,node_type NodeType) {
	while (true) {
		string value = nowToken();
		int TokPrec = GetTokPrecedence(nowToken());

		if (TokPrec < ExprPrec)
			return LHSChild;

		auto OPChild = OP();

		auto RHSChild = RHS();
		if (!RHSChild)
			return nullptr;

		int NextPrec = GetTokPrecedence(nowToken());
		if (TokPrec < NextPrec) {
			RHSChild->type = node_type::LHS;
			RHSChild = ParseBinOpRHS(TokPrec + 1, std::move(RHSChild),node_type::RHS);
			if (!RHSChild)
				return nullptr;
		}

		unique_ptr<Node>newLHSChild(new Node("", NodeType));
		newLHSChild->addChildNode(move(LHSChild));
		newLHSChild->addChildNode(move(OPChild));
		newLHSChild->addChildNode(move(RHSChild));
		LHSChild = move(newLHSChild);
	}
}
unique_ptr<Node> Parser::BinaryExpression(){
	unique_ptr<Node>binaryExpressionNode(new Node("", node_type::BinaryExpression));
	auto LHSChild = LHS();
	if (!LHSChild)return nullptr;
	if (tryEat(token_type::LOGICOR) || tryEat(token_type::LOGICAND) || tryEat(token_type::OR) ||
		tryEat(token_type::XOR) || tryEat(token_type::AND) || tryEat(token_type::EQUALITY) ||
		tryEat(token_type::NOTEQUAL) || tryEat(token_type::LESS) || tryEat(token_type::LESSEQUAL) ||
		tryEat(token_type::MORE) || tryEat(token_type::MOREEQUAL) || tryEat(token_type::LSHIFT) ||
		tryEat(token_type::RSHIFT) || tryEat(token_type::PLUS) || tryEat(token_type::MINUS) ||
		tryEat(token_type::STAR) || tryEat(token_type::SLASH) || tryEat(token_type::MOD) ||
		tryEat(token_type::NOT)) {
		auto Childs = move(ParseBinOpRHS(0, move(LHSChild), node_type::LHS));
		for (int child = 0; child != Childs->childNodes.size(); child++) {
			binaryExpressionNode->addChildNode(move(Childs->childNodes[child]));
		}
	}
	else {
		binaryExpressionNode->addChildNode(move(LHSChild));
		
	}
	return binaryExpressionNode;
}
unique_ptr<Node> Parser::LHS() {
	unique_ptr<Node>LHSNode(new Node("", node_type::LHS));
	auto PrimaryExpressionChild = PrimaryExpression();
	LHSNode->addChildNode(move(PrimaryExpressionChild));
	return LHSNode;
}
unique_ptr<Node> Parser::RHS() {
	unique_ptr<Node>LHSNode(new Node("", node_type::RHS));
	auto PrimaryExpressionChild = PrimaryExpression();
	LHSNode->addChildNode(move(PrimaryExpressionChild));
	return LHSNode;
}
unique_ptr<Node> Parser::OP() {
	string value = "";
	if (tryEat(token_type::LOGICOR)) {
		value = eat(token_type::LOGICOR);
	}
	else if (tryEat(token_type::LOGICAND)) {
		value = eat(token_type::LOGICAND);
	}
	else if (tryEat(token_type::OR)) {
		value = eat(token_type::OR);
	}
	else if (tryEat(token_type::XOR)) {
		value = eat(token_type::XOR);
	}
	else if (tryEat(token_type::AND)) {
		value = eat(token_type::AND);
	}
	else if (tryEat(token_type::EQUALITY)) {
		value = eat(token_type::EQUALITY);
	}
	else if (tryEat(token_type::NOTEQUAL)) {
		value = eat(token_type::NOTEQUAL);
	}
	else if (tryEat(token_type::LESS)) {
		value = eat(token_type::LESS);
	}
	else if (tryEat(token_type::LESSEQUAL)) {
		value = eat(token_type::LESSEQUAL);
	}
	else if (tryEat(token_type::MORE)) {
		value = eat(token_type::MORE);
	}
	else if (tryEat(token_type::MOREEQUAL)) {
		value = eat(token_type::MOREEQUAL);
	}
	else if (tryEat(token_type::LSHIFT)) {
		value = eat(token_type::LSHIFT);
	}
	else if (tryEat(token_type::RSHIFT)) {
		value = eat(token_type::RSHIFT);
	}
	else if (tryEat(token_type::PLUS)) {
		value = eat(token_type::PLUS);
	}
	else if (tryEat(token_type::MINUS)) {
		value = eat(token_type::MINUS);
	}
	else if (tryEat(token_type::STAR)) {
		value = eat(token_type::STAR);
	}
	else if (tryEat(token_type::SLASH)) {
		value = eat(token_type::SLASH);
	}
	else if (tryEat(token_type::MOD)) {
		value = eat(token_type::MOD);
	}
	else {
		value = eat(token_type::NOT);
	}

    unique_ptr<Node>opNode(new Node(value, node_type::OP));
	return opNode;
}

unique_ptr<Node> Parser::GroupedExpression(){
	unique_ptr<Node> groupedExpressionNode(new Node("", node_type::BlockExpression));
	
	unique_ptr<Node> tokenNodeLPAR(new Node(eat(token_type::LPAR), node_type::Token));
	groupedExpressionNode->addChildNode(move(tokenNodeLPAR));
	
	auto expressionStatementChild = ExpressionStatement();
	groupedExpressionNode->addChildNode(move(expressionStatementChild));
	
	unique_ptr<Node> tokenNodeRPAR(new Node(eat(token_type::RPAR), node_type::Token));
	groupedExpressionNode->addChildNode(move(tokenNodeRPAR));
	
	return groupedExpressionNode;
}
unique_ptr<Node> Parser::FunctionCall() {
	unique_ptr<Node> functionCallNode(new Node("", node_type::FunctionCall));

	auto functionIdentifierChild = FunctionIdentifier();	
	functionCallNode->addChildNode(move(functionIdentifierChild));

	unique_ptr<Node> tokenNodeLPAR(new Node(eat(token_type::LPAR), node_type::Token));
	functionCallNode->addChildNode(move(tokenNodeLPAR));

	auto callParameterListChild = CallParameterList();
	functionCallNode->addChildNode(move(callParameterListChild));

	unique_ptr<Node> tokenNodeRPAR(new Node(eat(token_type::RPAR), node_type::Token));
	functionCallNode->addChildNode(move(tokenNodeRPAR));
	
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

	unique_ptr<Node> tokenNodeIF(new Node(eat(token_type::IF), node_type::Token));
	ifExpressionNode->addChildNode(move(tokenNodeIF));

	auto conditionStatementChild = ConditionStatement();
	ifExpressionNode->addChildNode(move(conditionStatementChild));

	auto blockExpressionChild1 = BlockExpression();
	ifExpressionNode->addChildNode(move(blockExpressionChild1));

	if (tryEat(token_type::ELSE)) {
		unique_ptr<Node> tokenNodeELSE(new Node(eat(token_type::ELSE), node_type::Token));
		ifExpressionNode->addChildNode(move(tokenNodeELSE));
		if (tryEat(token_type::IF)) {
			auto ifExpressionChild = IfExpression();
			ifExpressionNode->addChildNode(move(ifExpressionChild));
		}
		else {
			auto blockExpressionChild3 = BlockExpression();
			ifExpressionNode->addChildNode(move(blockExpressionChild3));
		}
	}
	return ifExpressionNode;
}
unique_ptr<Node> Parser::ReturnExpression(){ 
	unique_ptr<Node> returnExpressionNode(new Node("", node_type::ReturnExpression));

	unique_ptr<Node> tokenNodeRETURN(new Node(eat(token_type::RETURN), node_type::Token));
	returnExpressionNode->addChildNode(move(tokenNodeRETURN));

	auto expressionStatementChild = ExpressionStatement();
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
	unique_ptr<Node> whileExpressionNode(new Node("", node_type::WhileExpression));

	unique_ptr<Node> tokenNodeWHILE(new Node(eat(token_type::WHILE), node_type::Token));
	whileExpressionNode->addChildNode(move(tokenNodeWHILE));

	auto expressionStatementChild = ExpressionStatement();
	whileExpressionNode->addChildNode(move(expressionStatementChild));

	auto blockExpressionChild = BlockExpression();
	whileExpressionNode->addChildNode(move(blockExpressionChild));

	return whileExpressionNode;
}
unique_ptr<Node> Parser::ForExpression(){
	unique_ptr<Node> forExpressionNode(new Node("", node_type::ForExpression));
	
	unique_ptr<Node> tokenNodeFOR(new Node(eat(token_type::FOR), node_type::Token));
	forExpressionNode->addChildNode(move(tokenNodeFOR));

	auto identifierChild = Identifier();
	forExpressionNode->addChildNode(move(identifierChild));


	unique_ptr<Node> tokenNodeIN(new Node(eat(token_type::IN), node_type::Token));
	forExpressionNode->addChildNode(move(tokenNodeIN));

	auto BinaryExpressionChild1 = BinaryExpression();
	forExpressionNode->addChildNode(move(BinaryExpressionChild1));

	unique_ptr<Node> tokenNodeDOUBLE_POINT(new Node(eat(token_type::DOUBLE_POINT), node_type::Token));
	forExpressionNode->addChildNode(move(tokenNodeDOUBLE_POINT));

	auto BinaryExpressionChild2 = BinaryExpression();
	forExpressionNode->addChildNode(move(BinaryExpressionChild2));

	auto blockExpressionChild = BlockExpression();
	forExpressionNode->addChildNode(move(blockExpressionChild));

	return forExpressionNode;
}
unique_ptr<Node> Parser::LoopExpression() {
	unique_ptr<Node> loopExpressionNode(new Node("", node_type::LoopExpression));

	unique_ptr<Node> tokenNodeLOOP(new Node(eat(token_type::LOOP), node_type::Token));
	loopExpressionNode->addChildNode(move(tokenNodeLOOP));

	auto blockExpressionChild = BlockExpression();
	loopExpressionNode->addChildNode(move(blockExpressionChild));

	return loopExpressionNode;
}
unique_ptr<Node> Parser::FunctionIdentifier(){
	auto value = eat(token_type::IDENTIFIER);
	unique_ptr<Node> functionIdentifier(new Node(value, node_type::FunctionIdentifier));
	return functionIdentifier;
}
unique_ptr<Node> Parser::ParameterList(){
	unique_ptr<Node>parameterListNode(new Node("", node_type::ParameterList));
	while (tryEat(token_type::IDENTIFIER)) {
		auto variableChild = Variable();
		parameterListNode->addChildNode(move(variableChild));

		unique_ptr<Node>tokenNodeCOLON(new Node(eat(token_type::COLON), node_type::ParameterList));
		parameterListNode->addChildNode(move(tokenNodeCOLON));

		auto dataTypeNode = DataType();
		parameterListNode->addChildNode(move(dataTypeNode));
		if (tryEat(token_type::RPAR))
			break;
		else {
			unique_ptr<Node>tokenNodeCOMMA(new Node(eat(token_type::COMMA), node_type::Token));
			parameterListNode->addChildNode(move(tokenNodeCOMMA));
		}
	}
	return parameterListNode;
}
unique_ptr<Node> Parser::CallParameterList(){
	unique_ptr<Node>callParameterListNode(new Node("", node_type::CallParameterList));
	while (tryEat(token_type::IDENTIFIER) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL)) {
		if (tryEat(token_type::IDENTIFIER)) {
			auto variableChild = Variable();
			callParameterListNode->addChildNode(move(variableChild));
			if (tryEat(token_type::RPAR))
				break;
			unique_ptr<Node>tokenNodeCOMMA(new Node(eat(token_type::COMMA), node_type::Token));
			callParameterListNode->addChildNode(move(tokenNodeCOMMA));

		}
		else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
			tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL)) {

			auto literalExpressionChild = LiteralExpression();
			callParameterListNode->addChildNode(move(literalExpressionChild));
			if (tryEat(token_type::RPAR))
				break;
			unique_ptr<Node>tokenNodeCOMMA(new Node(eat(token_type::COMMA), node_type::ParameterList));
			callParameterListNode->addChildNode(move(tokenNodeCOMMA));
		}
	}
	return callParameterListNode;
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
/*
* unique_ptr<Node> Parser::LogicalAndExpression(){
* 	unique_ptr<Node>logicalAndExpressionNode(new Node("", node_type::LogicalAndExpression));
* 	auto inclusiveOrExpressionChild = InclusiveOrExpression();
* 	auto logicalAndExpressionEChild = LogicalAndExpressionE();
* 	logicalAndExpressionNode->addChildNode(move(inclusiveOrExpressionChild));
* 	logicalAndExpressionNode->addChildNode(move(logicalAndExpressionEChild));
* 	return logicalAndExpressionNode;
* }
* unique_ptr<Node> Parser::InclusiveOrExpression(){ 
* 	unique_ptr<Node>inclusiveOrExpressionNode(new Node("", node_type::InclusiveOrExpression));
* 	auto exclusiveOrExpressionChild = ExclusiveOrExpression();
* 	auto inclusiveOrExpressionEChild = InclusiveOrExpressionE();
* 	inclusiveOrExpressionNode->addChildNode(move(exclusiveOrExpressionChild));
* 	inclusiveOrExpressionNode->addChildNode(move(inclusiveOrExpressionEChild));
* 	return inclusiveOrExpressionNode;
* }
* unique_ptr<Node> Parser::ExclusiveOrExpression() {
* 	unique_ptr<Node>exclusiveOrExpressionNode(new Node("", node_type::ExclusiveOrExpression));
* 	auto andExpressionChild = AndExpression();
* 	auto exclusiveOrExpressionEChild = ExclusiveOrExpressionE();
* 	exclusiveOrExpressionNode->addChildNode(move(andExpressionChild));
* 	exclusiveOrExpressionNode->addChildNode(move(exclusiveOrExpressionEChild));
* 	return exclusiveOrExpressionNode;
* }
* unique_ptr<Node> Parser::AndExpression() {
* 	unique_ptr<Node>andExpressionNode(new Node("", node_type::AndExpression));
* 	auto equalityExpressionChild = EqualityExpression();
* 	auto andExpressionEChild = AndExpressionE();
* 	andExpressionNode->addChildNode(move(equalityExpressionChild));
* 	andExpressionNode->addChildNode(move(andExpressionEChild));
* 	return andExpressionNode;
* }
* unique_ptr<Node> Parser::EqualityExpression() {
* 	unique_ptr<Node>equalityExpressionNode(new Node("", node_type::EqualityExpression));
* 	auto relationalExpressionChild = RelationalExpression();
* 	auto equalityExpressionENode = EqualityExpressionE();
* 	equalityExpressionNode->addChildNode(move(relationalExpressionChild));
* 	equalityExpressionNode->addChildNode(move(equalityExpressionENode));
* 	return equalityExpressionNode;
* }
* unique_ptr<Node> Parser::RelationalExpression() {
* 	unique_ptr<Node>relationalExpressionNode(new Node("", node_type::RelationalExpression));
* 	auto shiftExpressionChild = ShiftExpression();
* 	auto relationalExpressionEChild = RelationalExpressionE();
* 	relationalExpressionNode->addChildNode(move(shiftExpressionChild));
* 	relationalExpressionNode->addChildNode(move(relationalExpressionEChild));
* 	return relationalExpressionNode;
* }
* unique_ptr<Node> Parser::ShiftExpression() {
* 	unique_ptr<Node>shiftExpressionNode(new Node("", node_type::ShiftExpression));
* 	auto additiveExpressionChild = AdditiveExpression();
* 	auto shiftExpressionEChild = ShiftExpressionE();
* 	shiftExpressionNode->addChildNode(move(additiveExpressionChild));
* 	shiftExpressionNode->addChildNode(move(shiftExpressionEChild));
* 	return shiftExpressionNode;
* }
* unique_ptr<Node> Parser::AdditiveExpression() {
* 	unique_ptr<Node>additiveExpressionNode(new Node("", node_type::AdditiveExpression));
* 	auto multiplicativeExpressionChild = MultiplicativeExpression();
* 	auto additiveExpressionEChild = AdditiveExpressionE();
* 	additiveExpressionNode->addChildNode(move(multiplicativeExpressionChild));
* 	additiveExpressionNode->addChildNode(move(additiveExpressionEChild));
* 	return additiveExpressionNode;
* }
* unique_ptr<Node> Parser::MultiplicativeExpression(){
* 	unique_ptr<Node>multiplicativeExpressionNode(new Node("", node_type::MultiplicativeExpression));
* 	auto notExpressionChild = NotExpression();
* 	auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
* 	multiplicativeExpressionNode->addChildNode(move(notExpressionChild));
* 	multiplicativeExpressionNode->addChildNode(move(multiplicativeExpressionEChild));
* 	return multiplicativeExpressionNode;
* }
* unique_ptr<Node> Parser::NotExpression(){
* 	unique_ptr<Node>notExpressionENode(new Node("", node_type::NotExpression));
* 	unique_ptr<Node> child;
* 	if (tryEat(token_type::NOT)) {
* 		unique_ptr<Node>tokenNodeNOT(new Node(eat(token_type::NOT), node_type::Token));
* 		notExpressionENode->addChildNode(move(tokenNodeNOT));
* 
* 		child = PrimaryExpression();
* 	}
* 	//else if (tryEat(token_type::IDENTIFIER) || tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
* 	//	tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL) || tryEat(token_type::LPAR))
* 	else
* 		child = PrimaryExpression();
* 
* 	notExpressionENode->addChildNode(move(child));
* 	return notExpressionENode;
* }
*/
unique_ptr<Node> Parser::PrimaryExpression(){
	unique_ptr<Node>primaryExpressionNode(new Node("", node_type::PrimaryExpression));
	unique_ptr<Node> child;
	if (tryEat(token_type::IDENTIFIER)) {
		child = Variable();
		primaryExpressionNode->addChildNode(move(child));
	}	
	else if (tryEat(token_type::CHARACTER) || tryEat(token_type::NUMBER) ||
		tryEat(token_type::DOUBLE_NUMBER) || tryEat(token_type::BOOL))
	{
		child = LiteralExpression();
		primaryExpressionNode->addChildNode(move(child));
	}
	//else if (tryEat(token_type::LPAR)) {
	else {
		unique_ptr<Node>toeknNodeLPAR(new Node(eat(token_type::LPAR), node_type::Token));
		primaryExpressionNode->addChildNode(move(toeknNodeLPAR));

		child = BinaryExpression();
		primaryExpressionNode->addChildNode(move(child));

		unique_ptr<Node>toeknNodeRPAR(new Node(eat(token_type::RPAR), node_type::Token));
		primaryExpressionNode->addChildNode(move(toeknNodeRPAR));
	}
	return primaryExpressionNode;
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
		auto BinaryExpressionChild = BinaryExpression();
		conditionStatementNode->addChildNode(move(BinaryExpressionChild));
	}
	else {
		if (tryEat(token_type::IDENTIFIER)&&tryEat(token_type::LPAR, 1)) {
			auto functionCallChild = FunctionCall();
			conditionStatementNode->addChildNode(move(functionCallChild));
		}
		else
		{
			auto BinaryExpressionChild = BinaryExpression();
			conditionStatementNode->addChildNode(move(BinaryExpressionChild));
		}
	}
	return conditionStatementNode;
}
unique_ptr<Node> Parser::PRINTLN(){ 
	unique_ptr<Node>printlnNode(new Node("", node_type::PRINTLN));

	unique_ptr<Node>tokenNodePRINTLN(new Node(eat(token_type::PRINTLN), node_type::Token));
	printlnNode->addChildNode(move(tokenNodePRINTLN));

	unique_ptr<Node>tokenNodeLPAR(new Node(eat(token_type::LPAR), node_type::Token));
	printlnNode->addChildNode(move(tokenNodeLPAR));


	unique_ptr<Node>tokenNodeQUOTES(new Node(eat(token_type::QUOTES), node_type::Token));
	printlnNode->addChildNode(move(tokenNodeQUOTES));

	while (!tryEat(token_type::QUOTES))
	{
		lex->next_token();
	}

	unique_ptr<Node>tokenNodeQUOTES1(new Node(eat(token_type::QUOTES), node_type::Token));
	printlnNode->addChildNode(move(tokenNodeQUOTES1));

	//eat(token_type::COMMA);
	//auto callParameterListChild = CallParameterList();
	//printlnNode->addChildNode(move(callParameterListChild));

	unique_ptr<Node>tokenNodeRPAR(new Node(eat(token_type::RPAR), node_type::Token));
	printlnNode->addChildNode(move(tokenNodeRPAR));

	return printlnNode;
}
//×¢ÊÍÓï¾ä
unique_ptr<Node>Parser::COMMENT() {
	unique_ptr<Node>commentNode(new Node("", node_type::COMMENT));

	unique_ptr<Node>tokenNodeCOMMENT(new Node(eat(token_type::COMMENT), node_type::Token));
	commentNode->addChildNode(move(tokenNodeCOMMENT));

	return commentNode;
}

//Ïû³ý×óµÝ¹é
/*
* unique_ptr<Node>Parser::LogicalOrExpressionE() {
* 	unique_ptr<Node>BinaryExpressionENode(new Node("", node_type::BinaryExpressionE));
* 
* 	if (tryEat(token_type::LOGICOR)) {
* 
* 		unique_ptr<Node>tokenNodeLOGICOR(new Node(eat(token_type::LOGICOR), node_type::Token));
* 		BinaryExpressionENode->addChildNode(move(tokenNodeLOGICOR));
* 
* 		auto logicalAndExpressionChild = LogicalAndExpression();
* 		auto BinaryExpressionEChild = BinaryExpressionE();
* 		BinaryExpressionENode->addChildNode(move(logicalAndExpressionChild));
* 		BinaryExpressionENode->addChildNode(move(BinaryExpressionEChild));
* 		return BinaryExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::LogicalAndExpressionE() {
* 	unique_ptr<Node>logicalAndExpressionENode(new Node("", node_type::LogicalAndExpressionE));
* 
* 	if (tryEat(token_type::LOGICAND)) {
* 
* 		unique_ptr<Node>tokenNodeLOGICAND(new Node(eat(token_type::LOGICAND), node_type::Token));
* 		logicalAndExpressionENode->addChildNode(move(tokenNodeLOGICAND));
* 
* 		auto inclusiveOrExpressionChild = InclusiveOrExpression();
* 		auto logicalAndExpressionEChild = LogicalAndExpressionE();
* 		logicalAndExpressionENode->addChildNode(move(inclusiveOrExpressionChild));
* 		logicalAndExpressionENode->addChildNode(move(logicalAndExpressionEChild));
* 		return logicalAndExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::InclusiveOrExpressionE() {
* 	unique_ptr<Node>inclusiveOrExpressionENode(new Node("", node_type::InclusiveOrExpressionE));
* 	if (tryEat(token_type::OR)) {
* 		unique_ptr<Node>tokenNodeOR(new Node(eat(token_type::OR), node_type::Token));
* 		inclusiveOrExpressionENode->addChildNode(move(tokenNodeOR));
* 
* 		auto exclusiveOrExpressionChild = ExclusiveOrExpression();
* 		auto inclusiveOrExpressionEChild = InclusiveOrExpressionE();
* 		inclusiveOrExpressionENode->addChildNode(move(exclusiveOrExpressionChild));
* 		inclusiveOrExpressionENode->addChildNode(move(inclusiveOrExpressionEChild));
* 		return inclusiveOrExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::ExclusiveOrExpressionE() {
* 	unique_ptr<Node>exclusiveOrExpressionENode(new Node("", node_type::ExclusiveOrExpressionE));
* 	if (tryEat(token_type::XOR)) {
* 		unique_ptr<Node>tokenNodeXOR(new Node(eat(token_type::XOR), node_type::Token));
* 		exclusiveOrExpressionENode->addChildNode(move(tokenNodeXOR));
* 
* 		auto andExpressionChild = AndExpression();
* 		auto exclusiveOrExpressionEChild = ExclusiveOrExpressionE();
* 		exclusiveOrExpressionENode->addChildNode(move(andExpressionChild));
* 		exclusiveOrExpressionENode->addChildNode(move(exclusiveOrExpressionEChild));
* 		return exclusiveOrExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::AndExpressionE() {
* 	unique_ptr<Node>andExpressionENode(new Node("", node_type::AndExpressionE));
* 	if (tryEat(token_type::AND)) {
* 
* 		unique_ptr<Node>tokenNodeAND(new Node(eat(token_type::AND), node_type::Token));
* 		andExpressionENode->addChildNode(move(tokenNodeAND));
* 
* 		auto equalityExpressionChild = EqualityExpression();
* 		auto andExpressionEChild = AndExpressionE();
* 		andExpressionENode->addChildNode(move(equalityExpressionChild));
* 		andExpressionENode->addChildNode(move(andExpressionEChild));
* 		return andExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::EqualityExpressionE() {
* 	unique_ptr<Node>equalityExpressionENode(new Node("", node_type::EqualityExpressionE));
* 	if (tryEat(token_type::EQUALITY)) {
* 
* 		unique_ptr<Node>tokenNodeEQUALITY(new Node(eat(token_type::EQUALITY), node_type::Token));
* 		equalityExpressionENode->addChildNode(move(tokenNodeEQUALITY));
* 
* 		auto relationalExpressionChild = RelationalExpression();
* 		auto equalityExpressionEChild = EqualityExpressionE();
* 		equalityExpressionENode->addChildNode(move(relationalExpressionChild));
* 		equalityExpressionENode->addChildNode(move(equalityExpressionEChild));
* 		return equalityExpressionENode;
* 	}
* 	else if (tryEat(token_type::NOTEQUAL)) {
* 
* 		unique_ptr<Node>tokenNodeNOTEQUAL(new Node(eat(token_type::NOTEQUAL), node_type::Token));
* 		equalityExpressionENode->addChildNode(move(tokenNodeNOTEQUAL));
* 
* 		auto relationalExpressionChild = RelationalExpression();
* 		auto equalityExpressionEChild = EqualityExpressionE();
* 		equalityExpressionENode->addChildNode(move(relationalExpressionChild));
* 		equalityExpressionENode->addChildNode(move(equalityExpressionEChild));
* 		return equalityExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::RelationalExpressionE() {
* 	unique_ptr<Node>relationalExpressionENode(new Node("", node_type::RelationalExpressionE));
* 	if (tryEat(token_type::LESS)) {
* 
* 		unique_ptr<Node>tokenNodeLESS(new Node(eat(token_type::LESS), node_type::Token));
* 		relationalExpressionENode->addChildNode(move(tokenNodeLESS));
* 
* 		auto shiftExpressionChild = ShiftExpression();
* 		auto relationalExpressionEChild = RelationalExpressionE();
* 		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
* 		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
* 		return relationalExpressionENode;
* 	}
* 	else if (tryEat(token_type::MORE)) {
* 
* 		unique_ptr<Node>tokenNodeMORE(new Node(eat(token_type::MORE), node_type::Token));
* 		relationalExpressionENode->addChildNode(move(tokenNodeMORE));
* 
* 		auto shiftExpressionChild = ShiftExpression();
* 		auto relationalExpressionEChild = RelationalExpressionE();
* 		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
* 		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
* 		return relationalExpressionENode;
* 	}
* 	else if (tryEat(token_type::LESSEQUAL)) {
* 
* 		unique_ptr<Node>tokenNodeLESSEQUAL(new Node(eat(token_type::LESSEQUAL), node_type::Token));
* 		relationalExpressionENode->addChildNode(move(tokenNodeLESSEQUAL));
* 
* 		auto shiftExpressionChild = ShiftExpression();
* 		auto relationalExpressionEChild = RelationalExpressionE();
* 		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
* 		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
* 		return relationalExpressionENode;
* 	}
* 	else if (tryEat(token_type::MOREEQUAL)) {
* 
* 		unique_ptr<Node>tokenNodeMOREEQUAL(new Node(eat(token_type::MOREEQUAL), node_type::Token));
* 		relationalExpressionENode->addChildNode(move(tokenNodeMOREEQUAL));
* 
* 		auto shiftExpressionChild = ShiftExpression();
* 		auto relationalExpressionEChild = RelationalExpressionE();
* 		relationalExpressionENode->addChildNode(move(shiftExpressionChild));
* 		relationalExpressionENode->addChildNode(move(relationalExpressionEChild));
* 		return relationalExpressionENode;
* 	}
* 	else return nullptr;
* }
* unique_ptr<Node>Parser::ShiftExpressionE() {
* 	unique_ptr<Node>shiftExpressionENode(new Node("", node_type::ShiftExpressionE));
* 	if (tryEat(token_type::LSHIFT)) {
* 
* 		unique_ptr<Node>tokenNodeLSHIFT(new Node(eat(token_type::LSHIFT), node_type::Token));
* 		shiftExpressionENode->addChildNode(move(tokenNodeLSHIFT));
* 
* 		auto additiveExpressionChild = AdditiveExpression();
* 		auto shiftExpressionEChild = ShiftExpressionE();
* 		shiftExpressionENode->addChildNode(move(additiveExpressionChild));
* 		shiftExpressionENode->addChildNode(move(shiftExpressionEChild));
* 		return shiftExpressionENode;
* 	}
* 	else if (tryEat(token_type::RSHIFT)) {
		eat(token_type::RSHIFT);

		unique_ptr<Node>tokenNodeRSHIFT(new Node(eat(token_type::RSHIFT), node_type::Token));
		shiftExpressionENode->addChildNode(move(shiftExpressionENode));

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

		unique_ptr<Node>tokenNodePLUS(new Node(eat(token_type::PLUS), node_type::Token));
		additiveExpressionENode->addChildNode(move(tokenNodePLUS));

		auto multiplicativeExpressionChild = MultiplicativeExpression();
		auto additiveExpressionEChild = AdditiveExpressionE();
		additiveExpressionENode->addChildNode(move(multiplicativeExpressionChild));
		additiveExpressionENode->addChildNode(move(additiveExpressionEChild));
		return additiveExpressionENode;
	}
	else if (tryEat(token_type::MINUS)) {

		unique_ptr<Node>tokenNodeMINUS(new Node(eat(token_type::MINUS), node_type::Token));
		additiveExpressionENode->addChildNode(move(tokenNodeMINUS));

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

		unique_ptr<Node>tokenNodeSTAR(new Node(eat(token_type::STAR), node_type::Token));
		multiplicativeExpressionENode->addChildNode(move(tokenNodeSTAR));

		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else if (tryEat(token_type::SLASH)) {

		unique_ptr<Node>tokenNodeSLASH(new Node(eat(token_type::SLASH), node_type::Token));
		multiplicativeExpressionENode->addChildNode(move(tokenNodeSLASH));

		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else if (tryEat(token_type::PERCENT)) {

		unique_ptr<Node>tokenNodePERCENT(new Node(eat(token_type::PERCENT), node_type::Token));
		multiplicativeExpressionENode->addChildNode(move(tokenNodePERCENT));

		auto notExpressionChild = NotExpression();
		auto multiplicativeExpressionEChild = MultiplicativeExpressionE();
		multiplicativeExpressionENode->addChildNode(move(notExpressionChild));
		multiplicativeExpressionENode->addChildNode(move(multiplicativeExpressionEChild));
		return multiplicativeExpressionENode;
	}
	else return nullptr;
}
*/

void Parser::error(const string message)
{
	cout << "PARSE ERROR!" << message << " ";
	cout << "Current token: " << lex->current_token()->get_lexeme() << endl;

	throw logic_error(message);
	throw logic_error(message);
}
