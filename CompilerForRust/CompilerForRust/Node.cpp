#include "Node.h"


static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static unique_ptr<Module> TheModule;
static map<string, AllocaInst*> NamedValues;
static unique_ptr<legacy::FunctionPassManager> TheFPM;


static AllocaInst* CreateEntryBlockAlloca(Function* TheFunction,
	const std::string& VarName,Type* type) {
	IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
		TheFunction->getEntryBlock().begin());
	return TmpB.CreateAlloca(type, nullptr, VarName);
}

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

		NamedValues.clear();
		for (auto& Arg : TheFunction->args())
		{
			AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(),Arg.getType());
			Builder.CreateStore(&Arg, Alloca);
			NamedValues[Arg.getName()] = Alloca;
		}

		if (Value* RetVal = childNodes[bodyIdx]->codegen()) {
			Builder.CreateRet(RetVal);
			verifyFunction(*TheFunction);
			return TheFunction;
		}

		TheFunction->eraseFromParent();
		return nullptr;
	}
	case node_type::BlockExpression: {
		int i = 0;
		//保存变量列表
		vector<string>oldNames;
		vector<AllocaInst*>oldAllocas;
		for (auto k = NamedValues.begin(); k != NamedValues.end();k++) {
			oldNames.push_back(k->first);
			oldAllocas.push_back(k->second);
		}
		for (i ; i < childNodes.size(); i++) {
			if (childNodes[i]->type == node_type::Statements)
				break;
		}
		if (i == 0)return nullptr;

		Value* blockValue = childNodes[i]->codegen();

		//变量列表复原
		NamedValues.clear();
		for (int k = 0; k < oldNames.size(); k++) {
			NamedValues[oldNames[k]] = oldAllocas[k];
		}

		return blockValue;
	}
	case node_type::Statements: {
		int idx = 0;
		Value* returnValue;
		for (idx; idx < childNodes.size(); idx++) {
			node_type type = childNodes[idx]->type;
			Value* value = nullptr;
			if (type == node_type::Statement || type == node_type::IfExpression
				|| type == node_type::CycleExpression) {
				value = childNodes[idx]->codegen();
			}
			returnValue = value;
		}
		return returnValue;
	}
	case node_type::Statement: {
		return childNodes[0]->codegen();
	}
	case node_type::DeclarationStatement: {
		int nameIdx = -1, typeIdx = -1, rightIdx = -1;
		string nameVal, typeValue;
		for (int i = 0; i < childNodes.size(); i++) {
			node_type type = childNodes[i]->type;
			string value = childNodes[i]->value;
			if (type == node_type::VariableDefinition) {
				nameIdx = i;
				for (int j = 0; j < childNodes[i]->childNodes.size(); j++) {
					if (childNodes[i]->childNodes[j]->type == node_type::Variable) {
						nameVal = childNodes[i]->childNodes[j]->value;
					}
				}
			}
			else if (type == node_type::DataType) {
				typeIdx = i;
				typeValue = value;
			}
			else if (type == node_type::DeclarationRightStatement) {
				rightIdx = i;
			}
		}

		//原来的变量
		//std::vector<AllocaInst*> OldBindings;
		//改为循环语句负责重置变量表


		//找到所属函数
		Function* TheFunction = Builder.GetInsertBlock()->getParent();

		//变量类型
		Type* valType;
		if (typeIdx != -1) {
			valType = getType(typeValue);
		}
		else {
			valType = Type::getVoidTy(TheContext);
		}
		//初始值
		Value* init = nullptr;
		if (rightIdx != -1) {
			init = childNodes[rightIdx]->codegen();
		}

		AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, nameVal, valType);
		Builder.CreateStore(init, Alloca);
		NamedValues[nameVal] = Alloca;

		return nullptr;
	}
	case node_type::CycleExpression: {

	}
	case node_type::IfExpression: {
		int condIdx = -1, thenIdx = -1, elseIdx = -1;
		Value* condValue = nullptr;
		Value* thenValue = nullptr;
		Value* elseValue = nullptr;
		for (int i = 0; i < childNodes.size(); i++) {
			node_type type = childNodes[i]->type;
			if (type == node_type::ConditionStatement) {
				condIdx = i;
				thenIdx = i + 1;
			}
			else if (type == node_type::BlockExpression && thenIdx != -1) {
				elseIdx = i;
			}
		}

		condValue = childNodes[condIdx]->codegen();
		thenValue = childNodes[thenIdx]->codegen();
		if (elseIdx != -1) {
			elseValue = childNodes[elseIdx]->codegen();
		}

		//基本块

	}
	default:
		return ConstantFP::get(Type::getDoubleTy(TheContext), 1.0);
		break;
	}
}
