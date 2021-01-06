#include"Parser.h"
#include <iostream>
using namespace std;
map<int, string>path;
void InitMap() {
	path[0] = "intTest.txt";
	path[1] = "floatTest.txt";
	path[2] = "forTest.txt";
	path[3] = "functionCallTest.txt";
	path[4] = "ifTest.txt";
	path[5] = "whileTest.txt";

}
int main() {
	InitMap();
	string file = path[5];
	Parser parser(file);
	parser.parse();

	//生成IR
	parser.ast->tree->Init();
	parser.ast->tree->codegen();
	parser.ast->tree->print();

	//调用JIT
	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	parser.ast->tree->initEE();
	parser.ast->tree->runEE();
	return 0;
}