#pragma once
#include <string>
#include <iostream>

using namespace std;
static const char* tokenTypeList[] =
{
	"NUMBER",					//数字
	"DOUBLE_NUMBER",			//浮点数
	"IDENTIFIER",				//标识符
	"COMMENT",				//注释 "//"
	
	"UNDERSCORE",				//"_"
	"ABSTRACT",				//"abstract"
	"ALIGNOF",				//"alignof"
	"AS",						//"as"
	"BECOME",					//"become"
	"BOX",					//"box"
	"BREAK",					//"break"
	"CONST",					//"const"
	"CONTINUE",				//"continue"
	"CRATE",					//"crate"
	"DO",						//"do"
	"ELSE",					//"else"
	"ENUM",					//"enum"
	"EXTREN",					//"extern"
	"FALSE",					//"false"
	"FINAL",					//"final"
	"FN",						//"fn"
	"FOR",					//"for"
	"IF",						//"if"
	"IMPL",					//"impl"
	"IN",						//"in"
	"LET",					//"let"
	"LOOP",					//"loop"
	"MACRO",					//"macro"
	"MATCH",					//"match"
	"MOD",					//"mod"
	"MOVE",					//"move"
	"MUT",					//"mut"
	"OFFSETOF",				//"offsetof"
	"OVERRIDE",				//"override"
	"PRIV",					//"priv"
	"PROC",					//"proc"
	"PUB",					//"pub"
	"PURE",					//"pure"
	"REF",					//"ref"
	"RETURN",					//"return"
	"_SELF",					//"Self"
	"SELF",					//"self"
	"SIZEOF",					//"sizeof"
	"STATIC",					//"static"
	"STRUCT",					//"struct"
	"SUPER",					//"super"
	"TRAIT",					//"trait"
	"TRUE",					//"true"
	"TYPE",					//"type"
	"TYPEOF",					//"typeof"
	"UNSAFE",					//"unsafe"
	"UNSIZED",				//"unsized"
	"USE",					//"use"
	"VIRTUAL",				//"virtual"
	"WHERE",					//"where"
	"WHILE",					//"while"
	"YIELD",					//"yield"
	
	"DOUBLE_POINT",			// ..
	
	"SET", // =
	"EQUALITY", // ==
	
	"PLUS",			// +
	"MINUS",			// -
	"STAR",			// *
	"SLASH",			// /
	"PLUSEQUAL",		// +=
	"MINUSEQUAL",		// -=
	"STAREQUAL",		// *=
	"SLASHEQUAL",		// /=
	
	"LPAR",    // (
	"RPAR",    // )
	"LBRACE",  // {
	"RBRACE",  // }
	
	"QUOTES",  // "
	"COMMA",   // ,
	"SEMICOLON", // ;
	
	"MORE",			// >
	"LESS",			// <
	"OR",				// |
	"AND",			// &
	"MOREEQUAL",		// >=
	"LESSEQUAL",		// <=
	"OREQUAL",		// |=
	"ANDEQUAL",		// &=
	"RSHIFT",			// >>
	"RSHIFTEQUAL",	// >>=
	"LSHIFT",			// <<
	"LSHIFTEQUAL",	// <<
	"LOGICOR",		// ||
	"LOGICAND",		// &&
	"RETURNTYPE",		// ->
	
	"PRINTLN",
};
enum class token_type {
	NUMBER,					//数字
	DOUBLE_NUMBER,			//浮点数
	IDENTIFIER,				//标识符
	COMMENT,				//注释 "//"

	UNDERSCORE,				//"_"
	ABSTRACT,				//"abstract"
	ALIGNOF,				//"alignof"
	AS,						//"as"
	BECOME,					//"become"
	BOX,					//"box"
	BREAK,					//"break"
	CONST,					//"const"
	CONTINUE,				//"continue"
	CRATE,					//"crate"
	DO,						//"do"
	ELSE,					//"else"
	ENUM,					//"enum"
	EXTREN,					//"extern"
	FALSE,					//"false"
	FINAL,					//"final"
	FN,						//"fn"
	FOR,					//"for"
	IF,						//"if"
	IMPL,					//"impl"
	IN,						//"in"
	LET,					//"let"
	LOOP,					//"loop"
	MACRO,					//"macro"
	MATCH,					//"match"
	MOD,					//"mod"
	MOVE,					//"move"
	MUT,					//"mut"
	OFFSETOF,				//"offsetof"
	OVERRIDE,				//"override"
	PRIV,					//"priv"
	PROC,					//"proc"
	PUB,					//"pub"
	PURE,					//"pure"
	REF,					//"ref"
	RETURN,					//"return"
	_SELF,					//"Self"
	SELF,					//"self"
	SIZEOF,					//"sizeof"
	STATIC,					//"static"
	STRUCT,					//"struct"
	SUPER,					//"super"
	TRAIT,					//"trait"
	TRUE,					//"true"
	TYPE,					//"type"
	TYPEOF,					//"typeof"
	UNSAFE,					//"unsafe"
	UNSIZED,				//"unsized"
	USE,					//"use"
	VIRTUAL,				//"virtual"
	WHERE,					//"where"
	WHILE,					//"while"
	YIELD,					//"yield"

	DOUBLE_POINT,			// ..

	SET, // =
	EQUALITY, // ==

	PLUS,			// +
	MINUS,			// -
	STAR,			// *
	SLASH,			// /
	PLUSEQUAL,		// +=
	MINUSEQUAL,		// -=
	STAREQUAL,		// *=
	SLASHEQUAL,		// /=

	LPAR,    // (
	RPAR,    // )
	LBRACE,  // {
	RBRACE,  // }

	QUOTES,  // "
	COMMA,   // ,
	SEMICOLON, // ;

	MORE,			// >
	LESS,			// <
	OR,				// |
	AND,			// &
	MOREEQUAL,		// >=
	LESSEQUAL,		// <=
	OREQUAL,		// |=
	ANDEQUAL,		// &=
	RSHIFT,			// >>
	RSHIFTEQUAL,	// >>=
	LSHIFT,			// <<
	LSHIFTEQUAL,	// <<
	LOGICOR,		// ||
	LOGICAND,		// &&
	RETURNTYPE,		// ->
	PRINTLN,		
};

class Token
{
public:
	string lexeme;
	token_type type;
public:
	Token(const string& lexeme);

	token_type get_type();
	string get_lexeme();
	void add_lexeme(const string& lexeme);

	bool is_integer(const string& lexeme);
	bool is_double(const string& lexeme);
	bool is_correct_identifier(const string& lexeme);
};