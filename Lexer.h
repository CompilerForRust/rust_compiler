#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Token.h"
#include "Log.h"

using namespace std;

class Lexer
{
public:
	string code;
	vector<Token*> tokens;
	size_t current_token_index;
public:
	Lexer(const string file);

	void split();	//·ÖÎö×Ö·û´®
	bool is_split_symbol(char symbol);  //ÅÐ¶Ï·ûºÅ

	void next_token();
	token_type current_token_type();
	Token* current_token();
	string current_token_value();

	void print();   // Ð´ÈëÎÄ¼þ
};
