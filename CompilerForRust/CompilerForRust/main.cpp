#include"Parser.h"
#include <iostream>
using namespace std;

int main() {
	string file = "loopTest.txt";
	Parser parser(file);
	parser.parse();
	parser.ast->tree->Init();
	parser.ast->tree->codegen();
	parser.ast->tree->print();
	return 0;
}