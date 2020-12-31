#include "Lexer.h"

Lexer::Lexer(const string file)
{
	//打开代码文件
	ifstream file_with_code(file);
	if (file_with_code.is_open()) {
		char ch;
		//当读到EOF时仍执行一次循环
		while (!file_with_code.eof()) {
			file_with_code.get(ch);
			code += ch;
		}
		//对读到的内容进行修正
		//code[code.size() - 1] = EOF;
		code[code.size() - 1] = ' ';
		cout << "Code have been written to string" << endl;
	}

	current_token_index = 0;
}

void Lexer::split()
{
	string temp_token;
	auto it = code.begin();
	auto end = code.end();
	char symbol;

	//遍历code
	for (it; it != end; it++) {
		symbol = *it;
		if (is_split_symbol(symbol)) {
			//如果temp_token不为空
			if (!temp_token.empty()) {
				char next_sym = *it;
				if (temp_token == "println" && next_sym == '!')
				{
					temp_token += next_sym;

					it++;
					col++;
					symbol = *it;
				}
				tempCol = col - temp_token.size();
				Token* new_token = new Token(temp_token,line,tempCol);
				tokens.push_back(new_token);
				temp_token.clear();
			}
			if (symbol != ' ' && symbol != '\n' && symbol != '\t' && symbol != '=' &&
				symbol != '+' && symbol != '-' && symbol != '*' && symbol != '/' &&
				symbol != '<' && symbol != '>' && symbol != '|' && symbol != '&' &&
				symbol != '%' && symbol != '^' && symbol != '\'' && symbol != '!'
				)
			{
				temp_token = symbol;
				tempCol = col;
				Token* new_token = new Token(temp_token, line, tempCol);
				//Token* new_token = new Token(temp_token);
				tokens.push_back(new_token);
				temp_token.clear();
			}
			else if (symbol == '=') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size() + 1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
				}

			}
			else if (symbol == '\'') {
				temp_token = symbol;
				char next_sym = *(it + 1);
				char next_two_sym = *(it + 2);
				if (next_two_sym == '\'') {
					temp_token += next_sym;
					temp_token += next_two_sym;
					it += 2;
					col+=2;
				}
				tempCol = col - temp_token.size() + 1;
				Token* new_token = new Token(temp_token, line, tempCol);
				//Token* new_token = new Token(temp_token);
				tokens.push_back(new_token);
				temp_token.clear();
			}
			else if (symbol == '+') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '!') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '^') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '-')
			{
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else if (next_sym == '>') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '*') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '/') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else if (next_sym == '/') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					it++;
					col++;
					while (*it != '\n') {
						it++;
					}
					line++;
					col = 1;
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}

			}
			else if (symbol == '%') {
			it++;
			col++;
			char next_sym = *it;

			temp_token = symbol;
			if (next_sym == '=') {
				temp_token += next_sym;
				tempCol = col - temp_token.size()+1;
				Token* new_token = new Token(temp_token, line, tempCol);
				//Token* new_token = new Token(temp_token);
				tokens.push_back(new_token);
				temp_token.clear();
			}
			else {
				col--;
				tempCol = col;
				Token* new_token = new Token(temp_token, line, tempCol);
				//Token* new_token = new Token(temp_token);
				tokens.push_back(new_token);
				temp_token.clear();
				it--;
				
			}

			}
			else if (symbol == '<') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else if (next_sym == '<')
				{
					temp_token += next_sym;
					it++;
					col++;
					next_sym = *it;
					if (next_sym == '=') {
						temp_token += next_sym;
						tempCol = col - temp_token.size()+1;
						Token* new_token = new Token(temp_token, line, tempCol);
						//Token* new_token = new Token(temp_token);
						tokens.push_back(new_token);
						temp_token.clear();
					}
					else {
						col--;
						tempCol = col - temp_token.size()+1;
						Token* new_token = new Token(temp_token, line, tempCol);
						//Token* new_token = new Token(temp_token);
						tokens.push_back(new_token);
						temp_token.clear();
						it--;
						
					}
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}
			}

			else if (symbol == '>') {
				it++;
				col++;
				char next_sym = *it;

				temp_token = symbol;
				if (next_sym == '=') {
					temp_token += next_sym;
					tempCol = col - temp_token.size()+1;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
				}
				else if (next_sym == '>')
				{
					temp_token += next_sym;
					it++;
					col++;
					next_sym = *it;
					if (next_sym == '=') {
						temp_token += next_sym;
						tempCol = col - temp_token.size()+1;
						Token* new_token = new Token(temp_token, line, tempCol);
						//Token* new_token = new Token(temp_token);
						tokens.push_back(new_token);
						temp_token.clear();
					}
					else {
						col--;
						tempCol = col-temp_token.size();
						Token* new_token = new Token(temp_token, line, tempCol);
						//Token* new_token = new Token(temp_token);
						tokens.push_back(new_token);
						temp_token.clear();
						it--;
						
					}
				}
				else {
					col--;
					tempCol = col;
					Token* new_token = new Token(temp_token, line, tempCol);
					//Token* new_token = new Token(temp_token);
					tokens.push_back(new_token);
					temp_token.clear();
					it--;
					
				}
			}
			else if (symbol == '\n') {
			line++;
			col = 0;
			}
		}
		else {
				temp_token += symbol;
		}	
		col++;
	}
}

bool Lexer::is_split_symbol(char symbol)
{
	return symbol == '+' || symbol == '-' ||
		symbol == '/' || symbol == '*' ||
		symbol == '(' || symbol == ')' ||
		symbol == '{' || symbol == '}' ||
		symbol == '\"' || symbol == ',' ||
		symbol == ' ' || symbol == '=' ||
		symbol == '>' || symbol == '<' ||
		symbol == ';' || symbol == ',' ||
		symbol == '!' || symbol == '%' ||
		symbol == '^' || symbol == ':' ||
    symbol == '\'' ||
		symbol == '\t' || // tab
		symbol == '\n';   // 换行
}

void Lexer::print() // 写入文件
{
	cout_log("------ TOKENS ------");
	endl_log();

	for (const auto token : tokens) {
		cout_log(token->get_lexeme());
		cout_log(" WITH TYPE ");
		cout_log(tokenTypeList[(int)token->get_type()]);
		cout_log(" col:");
		cout_log(to_string(token->col));
		cout_log(" line:");
		cout_log(to_string(token->line));
		endl_log();
	}
	endl_log();

	cout << "Token vector writed successfully" << endl;
}

void Lexer::next_token()
{
	if (current_token_index < tokens.size() - 1) {
		current_token_index++;
	}
}

token_type Lexer::current_token_type()
{
	return tokens[current_token_index]->get_type();
}

Token* Lexer::current_token()
{
	return tokens[current_token_index];
}

string Lexer::current_token_value()
{
	return tokens[current_token_index]->get_lexeme();
}
