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
Value* IRError(string message) {
	cout<<message<<endl;
	return nullptr;
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
			AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, (string)Arg.getName(),Arg.getType());
			Builder.CreateStore(&Arg, Alloca);
			NamedValues[(string)Arg.getName()] = Alloca;
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
	case node_type::Variable: {
		Value* V = NamedValues[value];
		if (!V)return IRError("没有此变量");
		else return Builder.CreateLoad(V, value);
	}
	case node_type::CycleExpression: {
		return childNodes[0]->codegen();
	}
	case node_type::ForExpression: {
		string name;
		int starIdx = -1, endIdx = -1, bodyIdx = -1;
		for (int i = 0; i < childNodes.size(); i++) {
			auto type = childNodes[i]->type;
			auto value = childNodes[i]->value;
			if (type == node_type::Identifier) {
				name = value;
				starIdx = i + 2;
			}
			else if (type == node_type::BinaryExpression && starIdx != -1) {
				endIdx = i;
			}
			else if (type == node_type::BlockExpression) {
				bodyIdx = i;
			}
		}
		Function* TheFunction = Builder.GetInsertBlock()->getParent();

		AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, name,Type::getInt16Ty(TheContext));

		Value* StartVal = childNodes[starIdx]->codegen();
		if (!StartVal)
			return nullptr;

		Builder.CreateStore(StartVal, Alloca);

		BasicBlock* LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);

		Builder.CreateBr(LoopBB);

		Builder.SetInsertPoint(LoopBB);

		AllocaInst* OldVal = NamedValues[name];
		NamedValues[name] = Alloca;

		if (!childNodes[bodyIdx]->codegen())
			return nullptr;

		Value* StepVal = ConstantInt::get(Type::getInt16Ty(TheContext), 1);

		Value* EndVal = childNodes[endIdx]->codegen();
		if (!EndVal)
			return nullptr;

		Value* CurVar = Builder.CreateLoad(Alloca, name.c_str());
		Value* NextVar = Builder.CreateFAdd(CurVar, StepVal, "nextvar");
		Builder.CreateStore(NextVar, Alloca);

		Value* EndCond = new ICmpInst(*LoopBB, ICmpInst::ICMP_SLT, NextVar, EndVal);

		BasicBlock* AfterBB =
			BasicBlock::Create(TheContext, "afterloop", TheFunction);

		Builder.CreateCondBr(EndCond, LoopBB, AfterBB);

		Builder.SetInsertPoint(AfterBB);

		if (OldVal)
			NamedValues[name] = OldVal;
		else
			NamedValues.erase(name);

		return Constant::getNullValue(Type::getInt16Ty(TheContext));
	}
	case node_type::WhileExpression: {
		int condIdx = -1, bodyIdx = -1;
		for (int i = 0; i < childNodes.size(); i++) {
			auto type = childNodes[i]->type;
			if (type == node_type::ExpressionStatement) {
				condIdx = i;
			}
			else if (type == node_type::BlockExpression) {
				bodyIdx = i;
			}
		}
		Function* TheFunction = Builder.GetInsertBlock()->getParent();
		Value* CondVal = childNodes[condIdx]->codegen();
		CondVal = Builder.CreateICmpNE(CondVal,
			ConstantInt::get(Type::getInt1Ty(TheContext), 1), "cond");

		BasicBlock* BodyBlock = BasicBlock::Create(TheContext, "body", TheFunction);
		BasicBlock* AfterBlcok = BasicBlock::Create(TheContext, "after", TheFunction);

		Builder.CreateCondBr(CondVal, BodyBlock, AfterBlcok);

		Builder.SetInsertPoint(BodyBlock);
		if (!childNodes[bodyIdx]->codegen()) {
			return nullptr;
		}
		CondVal = childNodes[condIdx]->codegen();
		CondVal = Builder.CreateICmpNE(CondVal,
			ConstantInt::get(Type::getInt1Ty(TheContext), 1), "cond");
		Builder.CreateCondBr(CondVal, BodyBlock, AfterBlcok);

		Builder.SetInsertPoint(AfterBlcok);
		return Builder.CreateRetVoid();

	}
	case node_type::LoopExpression: {
		int bodyIdx = -1;
		for (int i = 0; i < childNodes.size(); i++) {
			auto type = childNodes[i]->type;
			if (type == node_type::BlockExpression) {
				bodyIdx = i;
			}
		}
		Function* TheFunction = Builder.GetInsertBlock()->getParent();
		BasicBlock* BodyBlock = BasicBlock::Create(TheContext, "body", TheFunction);
		Builder.CreateBr(BodyBlock);
		Builder.SetInsertPoint(BodyBlock);
		if (!childNodes[bodyIdx]->codegen())return nullptr;
		Builder.CreateBr(BodyBlock);
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

		
		condValue = Builder.CreateICmpNE(condValue,
			ConstantInt::get(Type::getInt1Ty(TheContext), 1), "cond");
		Function* TheFunction = Builder.GetInsertBlock()->getParent();
        //基本块
		BasicBlock* ThenBlock = BasicBlock::Create(TheContext, "ThenBlock", TheFunction);
		BasicBlock* ElseBlock = BasicBlock::Create(TheContext, "ElseBlock", TheFunction);
		

		Builder.CreateCondBr(condValue, ThenBlock, ElseBlock);

		Builder.SetInsertPoint(ThenBlock);
		thenValue = childNodes[thenIdx]->codegen();
		if (!thenValue)return nullptr;
		Builder.CreateRet(thenValue);


		Builder.SetInsertPoint(ElseBlock);
		if (elseIdx != -1) {
			elseValue = childNodes[elseIdx]->codegen();
		}
		if (!elseValue)return nullptr;
		Builder.CreateRet(ElseBlock);

	}
	case node_type::ConditionStatement: {
		Value* conditionStatementVal = childNodes[0]->codegen();
		return conditionStatementVal;
	}
	case node_type::GroupedExpression: {
		Value* groupedExpressionVal;
		for (int i = 0; i < childNodes.size(); i++) {
			if (childNodes[i]->type == node_type::ExpressionStatement)
				groupedExpressionVal = childNodes[i]->codegen();
		}
		return groupedExpressionVal;
	}
	case node_type::FunctionCall: {
		int argsIdx = -1;
		string name;
		for (int i = 0; i < childNodes.size(); i++) {
			auto type = childNodes[i]->type;
			auto value = childNodes[i]->value;
			if (type == node_type::FunctionIdentifier) {
				name = value;
			}
			else if (type == node_type::CallParameterList) {
				argsIdx = i;
			}
		}
		vector<Value*>Args;
		for (int k = 0; k < childNodes[argsIdx]->childNodes.size(); k++) {
			auto type = childNodes[argsIdx]->childNodes[k]->type;
			auto value = childNodes[argsIdx]->childNodes[k]->value;
			if (type == node_type::Variable) {
				Args.push_back(childNodes[argsIdx]->childNodes[k]->codegen());
			}
			else if (type == node_type::LiteralExpression) {
				Args.push_back(childNodes[argsIdx]->childNodes[k]->childNodes[0]->codegen());
			}
		}
		
		Function* Callee = TheModule->getFunction(name);
		if (!Callee)return IRError("函数不存在");
		if (Args.size() != Callee->arg_size())
			return IRError("参数数量不匹配");
		int j = 0;
		for (auto &calleeArg : Callee->args()) {
			if (calleeArg.getType() != Args[j]->getType())
				return IRError("参数类型不匹配");
			j++;
		}

		return Builder.CreateCall(Callee, Args, "call");
	}
	case node_type::LiteralExpression: {
		return childNodes[0]->codegen();
	}
	case node_type::DeclarationRightStatement: {
		return childNodes[0]->codegen();
	}
	case node_type::ExpressionStatement: {
		return childNodes[0]->codegen();
	}
	case node_type::ReturnExpression: {
		Value* returnVal = nullptr;
		for (int i = 0; i < childNodes.size(); i++) {
			if (childNodes[i]->type == node_type::ExpressionStatement) {
				returnVal = childNodes[i]->codegen();
			}
		}
		return Builder.CreateRet(returnVal);
	}
	case node_type::BreakExpression: {
		return Builder.CreateRetVoid();
	}
	default:
		return ConstantFP::get(Type::getDoubleTy(TheContext), 1.0);
		break;
	}
}
