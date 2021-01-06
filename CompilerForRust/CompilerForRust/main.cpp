#include"Parser.h"
#include <iostream>
using namespace std;

int main() {
	string file = "floatTest.txt";
	Parser parser(file);
	parser.parse();

	//����IR
	parser.ast->tree->Init();
	parser.ast->tree->codegen();
	parser.ast->tree->print();

	//����JIT
	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	parser.ast->tree->initEE();
	parser.ast->tree->runEE();
	return 0;
}