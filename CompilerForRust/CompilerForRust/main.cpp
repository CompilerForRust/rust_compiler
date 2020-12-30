#include "Parser.h"
#include <iostream>
using namespace std;

int main() {
	initMap();
	string file = "test.txt";
	Parser parser(file);
	parser.parse();
	return 0;
}