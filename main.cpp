#include "Token.h"
#include "Node.h"
#include "Lexer.h"
#include <iostream>
using namespace std;

int main() {
	string file = "test.txt";
	Lexer lexer(file);
	lexer.split();
	for (const auto token : lexer.tokens)
		cout << token->get_lexeme() << "    " << (int)token->type << endl;
	return 0;
}