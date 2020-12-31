#include "Node.h"

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value*> NamedValues;

Node::Node(const string value, node_type type) 
{
	this->type = type;
	this->value = value;
	this->childNodes.clear();
}
Node::Node(const string value, node_type type, vector<unique_ptr<Node>> childNodes)
{

	this->value = value;
	this->type = type;
	this->childNodes = move(childNodes);
}
Node::Node() {
	childNodes.clear();

};
Node::~Node() { childNodes.clear(); };
void Node::addChildNode(unique_ptr<Node> childNode)
{
	this->childNodes.push_back(move(childNode));
}

Type* getType(string returnVal) {
	if (returnVal == "i16" || returnVal == "u16") {
		return Type::getInt16Ty(TheContext);
	}
	else if (returnVal == "f32") {
		return Type::getFloatTy(TheContext);
	}
	else if (returnVal == "bool") {
		return Type::getInt1Ty(TheContext);
	}
	else {
		return Type::getInt8Ty(TheContext);
	}
}

Value* Node::codegen() {
	switch (type)
	{
	case node_type::FLOAT_LITERAL:
	{
		double doubleVal = atof(value.c_str());
		return ConstantFP::get(Type::getFloatTy(TheContext), doubleVal);
		break;
	}
	case node_type::INTEGER_LITERAL:
	{
		int intVal = atoi(value.c_str());
		return ConstantInt::get(Type::getInt16Ty(TheContext), intVal);
		break;
	}
	case node_type::BOOLEAN_LITERAL:
	{
		int boolVal = value == "true" ? 1 : 0;
		return ConstantInt::get(Type::getInt1Ty(TheContext), boolVal);
		break;
	}	
	case node_type::CHAR_STR_LITERAL:
		return ConstantInt::get(Type::getInt8Ty(TheContext), value[1]);
		break;
	case node_type::Program:
	{
		Value* functionDefinitionsChild = childNodes[0]->codegen();
		return nullptr;
	}
	case node_type::FunctionDefinitions: {
		for (int i = 0; i < childNodes.size(); i++) {
			childNodes[i]->codegen();
		}
		return nullptr;
	}
	case node_type::FunctionDefinition: {
		int nameIdx= -1, returnIdx = -1, argsIdx = -1, bodyIdx = -1;
		string nameVal, returnVal;
		for (int i = 0; i < childNodes.size(); i++) {
			node_type type = childNodes[i]->type;
			string value = childNodes[i]->value;
			if (type == node_type::FunctionIdentifier)
			{
				nameVal = value; nameIdx = i;
			}
			else if (type == node_type::DataType) {
				returnIdx = i; returnVal = value;
			}
			else if (type == node_type::ParameterList) {
				argsIdx = i;
			}
			else if (type == node_type::BlockExpression) {
				bodyIdx = i;
			}
		}
		Function* TheFunction = TheModule->getFunction(nameVal);
		if (!TheFunction)return TheFunction;
		Type* returnType;
		vector<Type*>args;
		vector<string>argNames;
		FunctionType* TheFunctionType;

		//确定返回值
		if (returnIdx != -1) {
			returnType = getType(returnVal);
		}
		else {
			returnType = Type::getVoidTy(TheContext);
		}
		
		//确定参数列表
		for (int j = 0; j < childNodes[argsIdx]->childNodes.size(); j++) {
			node_type type = childNodes[argsIdx]->childNodes[j]->type;
			string value = childNodes[argsIdx]->childNodes[j]->value;
			if (type == node_type::DataType)
				args.push_back(getType(value));
			else if (type == node_type::Variable)
				argNames.push_back(value);
		}

		//确定函数原型
		TheFunctionType = FunctionType::get(returnType, args, false);

		//确定函数
		TheFunction = Function::Create(TheFunctionType, Function::ExternalLinkage, nameVal, TheModule.get());

		//参数赋名
		int k = 0;
		for (auto& Arg : TheFunction->args()) {
			Arg.setName(argNames[k++]);
		}

		//函数体
		BasicBlock* BB = BasicBlock::Create(TheContext, "entry", TheFunction);
		Builder.SetInsertPoint(BB);

		for (auto& Arg : TheFunction->args())
			NamedValues[Arg.getName()] = &Arg;

		if (Value* RetVal = childNodes[bodyIdx]->codegen()) {
			Builder.CreateRet(RetVal);
			verifyFunction(*TheFunction);
			return TheFunction;
		}

		TheFunction->eraseFromParent();
		return nullptr;
	}
	default:
		return ConstantFP::get(Type::getDoubleTy(TheContext), 1.0);
		break;
	}
}
