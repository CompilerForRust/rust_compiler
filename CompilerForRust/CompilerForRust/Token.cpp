#include "Token.h"

Token::Token(const string& lexeme)
{
	this->lexeme = lexeme;

	this->type = get_type();
}
Token::Token(const string& lexeme, unsigned row, unsigned col)
{
	this->lexeme = lexeme;

	this->type = get_type();
	this->col = col;
	this->row = row;
}

token_type Token::get_type()
{
	if (is_integer(lexeme)) {
		return token_type::NUMBER;
	}
	else if (is_double(lexeme)) {
		return token_type::DOUBLE_NUMBER;
	}
	else if (lexeme == "//") {
		return token_type::COMMENT;
	}
	else if (lexeme == "_") {
		return token_type::UNDERSCORE;
	}
	else if (lexeme == "abstract") {
		return token_type::ABSTRACT;
	}
	else if (lexeme == "alignof") {
		return token_type::ALIGNOF;
	}
	else if (lexeme == "as") {
		return token_type::AS;
	}
	else if (lexeme == "become") {
		return token_type::BECOME;
	}
	else if (lexeme == "box") {
		return token_type::BOX;
	}
	else if (lexeme == "break") {
		return token_type::BREAK;
	}
	else if (lexeme == "const") {
		return token_type::CONST;
	}
	else if (lexeme == "continue") {
		return token_type::CONTINUE;
	}
	else if (lexeme == "crate") {
		return token_type::CRATE;
	}
	else if (lexeme == "do") {
		return token_type::DO;
	}
	else if (lexeme == "else") {
		return token_type::ELSE;
	}
	else if (lexeme == "enum") {
		return token_type::ENUM;
	}
	else if (lexeme == "extern") {
		return token_type::EXTREN;
	}
	else if (lexeme == "false") {
		return token_type::FALSE;
	}
	else if (lexeme == "final") {
		return token_type::FINAL;
	}
	else if (lexeme == "fn") {
		return token_type::FN;
	}
	else if (lexeme == "for") {
		return token_type::FOR;
	}
	else if (lexeme == "if") {
		return token_type::IF;
	}
	else if (lexeme == "impl") {
		return token_type::IMPL;
	}
	else if (lexeme == "in") {
		return token_type::IN;
	}
	else if (lexeme == "let") {
		return token_type::LET;
	}
	else if (lexeme == "loop") {
		return token_type::LOOP;
	}
	else if (lexeme == "macro") {
		return token_type::MACRO;
	}
	else if (lexeme == "match") {
		return token_type::MATCH;
	}
	else if (lexeme == "mod") {
		return token_type::MOD;
	}
	else if (lexeme == "move") {
		return token_type::MOVE;
	}
	else if (lexeme == "mut") {
		return token_type::MUT;
	}
	else if (lexeme == "offsetof") {
		return token_type::OFFSETOF;
	}
	else if (lexeme == "override") {
		return token_type::OVERRIDE;
	}
	else if (lexeme == "priv") {
		return token_type::PRIV;
	}
	else if (lexeme == "proc") {
		return token_type::PROC;
	}
	else if (lexeme == "pub") {
		return token_type::PUB;
	}
	else if (lexeme == "pure") {
		return token_type::PURE;
	}
	else if (lexeme == "ref") {
		return token_type::REF;
	}
	else if (lexeme == "return") {
		return token_type::RETURN;
	}
	else if (lexeme == "Self") {
		return token_type::_SELF;
	}
	else if (lexeme == "self") {
		return token_type::SELF;
	}
	else if (lexeme == "sizeof") {
		return token_type::SIZEOF;
	}
	else if (lexeme == "static") {
		return token_type::STATIC;
	}
	else if (lexeme == "struct") {
		return token_type::STRUCT;
	}
	else if (lexeme == "super") {
		return token_type::SUPER;
	}
	else if (lexeme == "trait") {
		return token_type::TRAIT;
	}
	else if (lexeme == "true") {
		return token_type::TRUE;
	}
	else if (lexeme == "type") {
		return token_type::TYPE;
	}
	else if (lexeme == "typeof") {
		return token_type::TYPEOF;
	}
	else if (lexeme == "unsafe") {
		return token_type::UNSAFE;
	}
	else if (lexeme == "unsized") {
		return token_type::UNSIZED;
	}
	else if (lexeme == "use") {
		return token_type::USE;
	}
	else if (lexeme == "virtual") {
		return token_type::VIRTUAL;
	}
	else if (lexeme == "where") {
		return token_type::WHERE;
	}
	else if (lexeme == "while") {
		return token_type::WHILE;
	}
	else if (lexeme == "yield") {
		return token_type::YIELD;
	}
	else if (lexeme == "..") {
		return token_type::DOUBLE_POINT;
	}
	else if (lexeme == "=") {
		return token_type::SET;
	}
	else if (lexeme == "==") {
		return token_type::EQUALITY;
	}
	else if (lexeme == "+") {
		return token_type::PLUS;
	}
	else if (lexeme == "-") {
		return token_type::MINUS;
	}
	else if (lexeme == "*") {
		return token_type::STAR;
	}
	else if (lexeme == "/") {
		return token_type::SLASH;
	}
	else if (lexeme == "%") {
	return token_type::PERCENT;
	}
	else if (lexeme == "%=") {
	return token_type::PERCENTEQUAL;
	}
	else if (lexeme == "+=") {
	return token_type::PLUSEQUAL;
	}
	else if (lexeme == "-=") {
	return token_type::MINUSEQUAL;
	}
	else if (lexeme == "*=") {
	return token_type::STAREQUAL;
	}
	else if (lexeme == "/=") {
	return token_type::SLASHEQUAL;
	}
	else if (lexeme == "(") {
		return token_type::LPAR;
	}
	else if (lexeme == ")") {
		return token_type::RPAR;
	}
	else if (lexeme == "{") {
		return token_type::LBRACE;
	}
	else if (lexeme == "}") {
		return token_type::RBRACE;
	}
	else if (lexeme == "\"") {
		return token_type::QUOTES;
	}
	else if (lexeme == "'") {
	return token_type::SINGLEQUOTES;
	}
	else if (lexeme == ",") {
		return token_type::COMMA;
	}
	else if (lexeme == ";") {
		return token_type::SEMICOLON;
	}
	else if (lexeme == ">") {
		return token_type::MORE;
	}
	else if (lexeme == "<") {
		return token_type::LESS;
	}
	else if (lexeme == "|") {
		return token_type::OR;
	}
	else if (lexeme == "&") {
		return token_type::AND;
	}
	else if (lexeme == ">=") {
	return token_type::MOREEQUAL;
	}
	else if (lexeme == "<=") {
	return token_type::LESSEQUAL;
	}
	else if (lexeme == "|=") {
	return token_type::OREQUAL;
	}
	else if (lexeme == "&=") {
	return token_type::ANDEQUAL;
	}
	else if (lexeme == ">>") {
	return token_type::RSHIFT;
	}
	else if (lexeme == "<<") {
	return token_type::LSHIFT;
	}
	else if (lexeme == ">>=") {
	return token_type::RSHIFTEQUAL;
	}
	else if (lexeme == "<<=") {
	return token_type::LSHIFTEQUAL;
	}
	else if (lexeme == "||") {
	return token_type::LOGICOR;
	}
	else if (lexeme == "&&") {
	return token_type::LOGICAND;
	}
	else if (lexeme == "->") {
	return token_type::RETURNTYPE;
	}
	else if (lexeme == "!") {
	return token_type::NOT;
	}
	else if (lexeme == "!=") {
	return token_type::NOTEQUAL;
	}
	else if (lexeme == "^") {
	return token_type::XOR;
	}
	else if (lexeme == "^=") {
	return token_type::XOREQUAL;
	}
	else if (lexeme == ":") {
	return token_type::COLON;
	}
	else if (lexeme == "i16") {
	return token_type::I16;
	}
	else if (lexeme == "u16") {
	return token_type::U16;
	}
	else if (lexeme == "char") {
	return token_type::CHAR;
	}
	else if (lexeme == "bool") {
	return token_type::BOOL;
	}
	else if (lexeme == "f32") {
	return token_type::F32;
	}
	else if (lexeme == "println!") {
		return token_type::PRINTLN;
	}
	else if (lexeme.size()==3&& lexeme[0] == '\''&& lexeme[2] == '\'') {
	return token_type::CHARACTER;
    }
	else if (is_correct_identifier(lexeme)) {
		return token_type::IDENTIFIER;
	}
	else {
		throw logic_error("Lexical error! Incorrect identifier " + lexeme + " !");
	}
}

void Token::add_lexeme(const string& lexeme) {
	this->lexeme += lexeme;
}

//测试整数
bool Token::is_integer(const string& lexeme)
{
	for (const auto symbol : lexeme) {
		if (symbol < '0' || symbol > '9') {
			return false;
		}
	}
	return true;
}

//测试浮点数
bool Token::is_double(const string& lexeme)
{
	int dot_num = 0;
	for (const auto symbol : lexeme) {
		if (symbol >= '0' && symbol <= '9' || symbol == '.') 
		{
			if (symbol == '.')
				dot_num++;
			continue;
		}
		else
			return false;
	}
	if (dot_num > 1)
		return false;
	return true;
}

//测试标识符
bool Token::is_correct_identifier(const string& lexeme)
{
	if (!isalpha(lexeme[0]) && lexeme[0] != '_') {
		return false;
	}

	for (const auto& symbol : lexeme) {
		if (!isalpha(symbol) && !isdigit(symbol) && symbol != '_') {
			return false;
		}
	}

	return true;
}

string Token::get_lexeme()
{
	return this->lexeme;
}