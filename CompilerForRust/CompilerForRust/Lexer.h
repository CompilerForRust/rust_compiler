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
	unsigned int line = 1;		//行号
	unsigned int col = 1;		//列号
	unsigned int tempCol = col;
public:
	Lexer(const string file);

	void split();	//分析字符串
	bool is_split_symbol(char symbol);  //判断符号

	void next_token();
	token_type current_token_type();
	Token* current_token();
	string current_token_value();

	void print();   // 写入文件
};
