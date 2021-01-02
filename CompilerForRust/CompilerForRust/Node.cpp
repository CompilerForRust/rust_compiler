#include "Node.h"
static std::unique_ptr<LLVMContext> TheContext;
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<IRBuilder<>> Builder;
static map<string, AllocaInst*> NamedValues;
void Node::Init() {
	// Open a new context and module.
	TheContext = std::make_unique<LLVMContext>();
	TheModule = std::make_unique<Module>("my cool jit", *TheContext);
	// Create a new *Builder for the module.
	Builder = std::make_unique<IRBuilder<>>(*TheContext);
}
void Node::print() {
	TheModule->print(errs(), nullptr);
}
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
Type* Node::getType(string returnVal) {
	if (returnVal == "i16" || returnVal == "u16") {
		return Type::getInt16Ty(*TheContext);
	}
	else if (returnVal == "f32") {
		return Type::getFloatTy(*TheContext);
	}
	else if (returnVal == "bool") {
		return Type::getInt1Ty(*TheContext);
	}
	else {
		return Type::getInt8Ty(*TheContext);
	}
}

unique_ptr<Node> LogError(const char* Str) {
	fprintf(stderr, "Error:%s\n", Str);
	return nullptr;
}
Value* LogErrorVV(const char* Str) {
	LogError(Str);
	return nullptr;
}
/*
将String转换为hash值用于switch case
*/
typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

hash_t hash_(char const* str)
{
	hash_t ret{ basis };

	while (*str) {
		ret ^= *str;
		ret *= prime;
		str++;
	}

	return ret;
}
constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
	return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}

Value* Node::codegen() {
	switch (type)
	{
	case node_type::FLOAT_LITERAL:
	{
		double doubleVal = atof(value.c_str());
		return ConstantFP::get(Type::getFloatTy(*TheContext), doubleVal);
		break;
	}
	case node_type::INTEGER_LITERAL:
	{
		int intVal = atoi(value.c_str());
		return ConstantInt::get(Type::getInt16Ty(*TheContext), intVal);
		break;
	}
	case node_type::BOOLEAN_LITERAL:
	{
		int boolVal = value == "true" ? 1 : 0;
		return ConstantInt::get(Type::getInt1Ty(*TheContext), boolVal);
		break;
	}	
	case node_type::CHAR_STR_LITERAL:
		return ConstantInt::get(Type::getInt8Ty(*TheContext), value[1]);
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
		if (TheFunction)return TheFunction;
		Type* returnType;
		vector<Type*>args;
		vector<string>argNames;
		FunctionType* TheFunctionType;

		//确定返回值
		if (returnIdx != -1) {
			returnType = getType(returnVal);
		}
		else {
			returnType = Type::getVoidTy(*TheContext);
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
		BasicBlock* BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
		Builder->SetInsertPoint(BB);

		NamedValues.clear();
		for (auto& Arg : TheFunction->args())
		{
			AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, (string)Arg.getName(),Arg.getType());
			Builder->CreateStore(&Arg, Alloca);
			NamedValues[(string)Arg.getName()] = Alloca;
		}

		if (Value* RetVal = childNodes[bodyIdx]->codegen()) {
			Builder->CreateRet(RetVal);
			verifyFunction(*TheFunction);
			return TheFunction;
		}

		TheFunction->eraseFromParent();
		return nullptr;
	}

	//改过文法之后assignmentExpression现在应该只有->vari+assignOp+BinOp这种类型了吧
	case node_type::AssignmentExpression: {
		Value* L = childNodes[0]->codegen();
		Value* R = childNodes[2]->codegen();	
		Value* Tmp;
		string assignOpValue = childNodes[1]->value;
		const char* asValue = assignOpValue.data();
		switch (hash_(asValue))
		{
		case(hash_compile_time("=")): 
			Builder->CreateStore(R, L);
			return nullptr;
		case(hash_compile_time("*=")):
			Tmp = Builder->CreateFMul(L, R, "multmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("/=")):
			Tmp = Builder->CreateFDiv(L, R, "divtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("%=")):
			Tmp = Builder->CreateSRem(L, R, "remtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("+=")):
			Tmp = Builder->CreateFAdd(L, R, "addtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("-=")):
			Tmp = Builder->CreateFSub(L, R, "subtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("<<=")):
			Tmp = Builder->CreateShl(L, R, "shltmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time(">>=")):
			Tmp = Builder->CreateLShr(L, R, "LShrtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("&=")):
			Tmp = Builder->CreateAnd(L, R, "andtmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("^=")):
			Tmp = Builder->CreateXor(L, R, "xortmp");
			return Builder->CreateStore(R, Tmp);
		case(hash_compile_time("|=")):
			Tmp = Builder->CreateOr(L, R, "ortmp");
			return Builder->CreateStore(R, Tmp);
		default:
			return LogErrorVV("invalid assignment operator");

		}

	}
	//后来我发现LHS可以再分LHS OP RHS ，不知道还有没有其他情况，目前操作就是加个判断看看是什么，感觉分的时候应该有一个binaryexpression会更好看一些，但好像因为左递归的问题去掉了
	case node_type::LHS: {
		node_type firstType = childNodes[0]->type;
		string returnValue;
		//直接从Binary Expression粘过来了
		if (firstType==node_type::LHS) {
			Value* L = childNodes[0]->codegen();
			Value* R = childNodes[2]->codegen();
			if (!L || !R)
				return nullptr;

			string Op = childNodes[1]->value;
			const char* op = Op.data();

			switch (hash_(op)) {
			case hash_compile_time("+"):
				return Builder->CreateFAdd(L, R, "addtmp");
			case hash_compile_time("-"):
				return Builder->CreateFSub(L, R, "subtmp");
			case hash_compile_time("*"):
				return Builder->CreateFMul(L, R, "multmp");
			case hash_compile_time("/"):
				return Builder->CreateFDiv(L, R, "divtmp");
			case hash_compile_time("%"):
				return Builder->CreateSRem(L, R, "remtmp");
			case hash_compile_time("<<"):
				return Builder->CreateShl(L, R, "shltmp");
			case hash_compile_time(">>"):
				return Builder->CreateLShr(L, R, "lshrtmp");
			case hash_compile_time(">"):
				return Builder->CreateFCmpUGT(L, R, "ugttmp");
			case hash_compile_time(">="):
				return Builder->CreateFCmpUGE(L, R, "ugetmp");
			case hash_compile_time("<"):
				return Builder->CreateFCmpULT(L, R, "ulttmp");
			case hash_compile_time("<="):
				return Builder->CreateFCmpULE(L, R, "uletmp");
			case hash_compile_time("||"):
				return Builder->CreateOr(L, R, "ortmp");
			case hash_compile_time("&&"):
				return Builder->CreateAnd(L, R, "andtmp");
			case hash_compile_time("=="):
				return Builder->CreateICmpEQ(L, R, "equtmp");
			case hash_compile_time("!="):
				return Builder->CreateICmpNE(L, R, "neqtmp");
			case hash_compile_time("^"):
				return Builder->CreateXor(L, R, "xortmp");
				//按位与或找不到函数就先用与或凑合了
			case hash_compile_time("|"):
				return Builder->CreateOr(L, R, "ortmp");
			case hash_compile_time("&"):
				return Builder->CreateAnd(L, R, "andtmp");


				//二进制表达式只有！没有写，考虑了一下不应该在这里去写

			default:
				return LogErrorVV("invalid binary operator");
			}
		}

		//primaryExpression
		else
		{
			return childNodes[0]->codegen();
		}
	}
	case node_type::RHS: {
		for (int i = 0; i < childNodes.size(); i++) {
			childNodes[i]->codegen();
		}
		return nullptr;
	}
	case node_type::PrimaryExpression: {
		//1.带括号则取中间
		node_type type = childNodes[0]->type;
		if (type == node_type::Token) {
			return childNodes[1]->codegen();
		}
		//2.literalexpression或者变量
		else 
		{
			return childNodes[0]->codegen();
		}
	}
	case node_type::BinaryExpression: {
		//只有一个LHS时
		if (childNodes.size() == 1) {
			return childNodes[0]->codegen();
		}
		else
		{
			Value* L = childNodes[0]->codegen();
			Value* R = childNodes[2]->codegen();
			if (!L || !R)
				return nullptr;
			
			string Op = childNodes[1]->value;
			const char* op = Op.data();

			switch (hash_(op)) {
			case hash_compile_time("+"):
				return Builder->CreateFAdd(L, R, "addtmp");
			case hash_compile_time("-"):
				return Builder->CreateFSub(L, R, "subtmp");
			case hash_compile_time("*"):
				return Builder->CreateFMul(L, R, "multmp");
			case hash_compile_time("/"):
				return Builder->CreateFDiv(L, R, "divtmp");
			case hash_compile_time("%"):
				return Builder->CreateSRem(L, R, "remtmp");
			case hash_compile_time("<<"):
				return Builder->CreateShl(L, R, "shltmp");
			case hash_compile_time(">>"):
				return Builder->CreateLShr(L, R, "lshrtmp");
			case hash_compile_time(">"):
				return Builder->CreateFCmpUGT(L, R, "ugttmp");
			case hash_compile_time(">="):
				return Builder->CreateFCmpUGE(L, R, "ugetmp");
			case hash_compile_time("<"):
				return Builder->CreateFCmpULT(L, R, "ulttmp");
			case hash_compile_time("<="):
				return Builder->CreateFCmpULE(L, R, "uletmp");
			case hash_compile_time("||"):
				return Builder->CreateOr(L, R, "ortmp");
			case hash_compile_time("&&"):
				return Builder->CreateAnd(L, R, "andtmp");
			case hash_compile_time("=="):
				return Builder->CreateICmpEQ(L, R, "equtmp");
			case hash_compile_time("!="):
				return Builder->CreateICmpNE(L, R, "neqtmp");
			case hash_compile_time("^"):
				return Builder->CreateXor(L, R, "xortmp");
				//按位与或找不到函数就先用与或了
			case hash_compile_time("|"):
				return Builder->CreateOr(L, R, "ortmp");
			case hash_compile_time("&"):
				return Builder->CreateAnd(L, R, "andtmp");

			

			default:
				return LogErrorVV("invalid binary operator");
			}
		}
		
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
		Function* TheFunction = Builder->GetInsertBlock()->getParent();

		//变量类型
		Type* valType;
		if (typeIdx != -1) {
			valType = getType(typeValue);
		}
		else {
			valType = Type::getVoidTy(*TheContext);
		}
		//初始值
		Value* init = nullptr;
		if (rightIdx != -1) {
			init = childNodes[rightIdx]->codegen();
		}

		AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, nameVal, valType);
		Builder->CreateStore(init, Alloca);
		NamedValues[nameVal] = Alloca;

		return nullptr;
	}
	case node_type::Variable: {
		Value* V = NamedValues[value];
		if (!V)return IRError("None Varibal");
		else return Builder->CreateLoad(V, value);
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
		Function* TheFunction = Builder->GetInsertBlock()->getParent();

		AllocaInst* Alloca = CreateEntryBlockAlloca(TheFunction, name,Type::getInt16Ty(*TheContext));

		Value* StartVal = childNodes[starIdx]->codegen();
		if (!StartVal)
			return nullptr;

		Builder->CreateStore(StartVal, Alloca);

		BasicBlock* LoopBB = BasicBlock::Create(*TheContext, "loop", TheFunction);

		Builder->CreateBr(LoopBB);

		Builder->SetInsertPoint(LoopBB);

		AllocaInst* OldVal = NamedValues[name];
		NamedValues[name] = Alloca;

		if (!childNodes[bodyIdx]->codegen())
			return nullptr;

		Value* StepVal = ConstantInt::get(Type::getInt16Ty(*TheContext), 1);

		Value* EndVal = childNodes[endIdx]->codegen();
		if (!EndVal)
			return nullptr;

		Value* CurVar = Builder->CreateLoad(Alloca, name.c_str());
		Value* NextVar = Builder->CreateFAdd(CurVar, StepVal, "nextvar");
		Builder->CreateStore(NextVar, Alloca);

		Value* EndCond = new ICmpInst(*LoopBB, ICmpInst::ICMP_SLT, NextVar, EndVal);

		BasicBlock* AfterBB =
			BasicBlock::Create(*TheContext, "afterloop", TheFunction);

		Builder->CreateCondBr(EndCond, LoopBB, AfterBB);

		Builder->SetInsertPoint(AfterBB);

		if (OldVal)
			NamedValues[name] = OldVal;
		else
			NamedValues.erase(name);

		return Constant::getNullValue(Type::getInt16Ty(*TheContext));
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
		Function* TheFunction = Builder->GetInsertBlock()->getParent();
		Value* CondVal = childNodes[condIdx]->codegen();
		CondVal = Builder->CreateICmpNE(CondVal,
			ConstantInt::get(Type::getInt1Ty(*TheContext), 1), "cond");

		BasicBlock* BodyBlock = BasicBlock::Create(*TheContext, "body", TheFunction);
		BasicBlock* AfterBlcok = BasicBlock::Create(*TheContext, "after", TheFunction);

		Builder->CreateCondBr(CondVal, BodyBlock, AfterBlcok);

		Builder->SetInsertPoint(BodyBlock);
		if (!childNodes[bodyIdx]->codegen()) {
			return nullptr;
		}
		CondVal = childNodes[condIdx]->codegen();
		CondVal = Builder->CreateICmpNE(CondVal,
			ConstantInt::get(Type::getInt1Ty(*TheContext), 1), "cond");
		Builder->CreateCondBr(CondVal, BodyBlock, AfterBlcok);

		Builder->SetInsertPoint(AfterBlcok);
		return Builder->CreateRetVoid();

	}
	case node_type::LoopExpression: {
		int bodyIdx = -1;
		for (int i = 0; i < childNodes.size(); i++) {
			auto type = childNodes[i]->type;
			if (type == node_type::BlockExpression) {
				bodyIdx = i;
			}
		}
		Function* TheFunction = Builder->GetInsertBlock()->getParent();
		BasicBlock* BodyBlock = BasicBlock::Create(*TheContext, "body", TheFunction);
		Builder->CreateBr(BodyBlock);
		Builder->SetInsertPoint(BodyBlock);
		if (!childNodes[bodyIdx]->codegen())return nullptr;
		Builder->CreateBr(BodyBlock);
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

		
		condValue = Builder->CreateICmpNE(condValue,
			ConstantInt::get(Type::getInt1Ty(*TheContext), 1), "cond");
		Function* TheFunction = Builder->GetInsertBlock()->getParent();
        //基本块
		BasicBlock* ThenBlock = BasicBlock::Create(*TheContext, "ThenBlock", TheFunction);
		BasicBlock* ElseBlock = BasicBlock::Create(*TheContext, "ElseBlock", TheFunction);
		

		Builder->CreateCondBr(condValue, ThenBlock, ElseBlock);

		Builder->SetInsertPoint(ThenBlock);
		thenValue = childNodes[thenIdx]->codegen();
		if (!thenValue)return nullptr;
		Builder->CreateRet(thenValue);


		Builder->SetInsertPoint(ElseBlock);
		if (elseIdx != -1) {
			elseValue = childNodes[elseIdx]->codegen();
		}
		if (!elseValue)return nullptr;
		Builder->CreateRet(ElseBlock);

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
		if (!Callee)return IRError("Function not exist");
		if (Args.size() != Callee->arg_size())
			return IRError("Parameter number mismatch");
		int j = 0;
		for (auto &calleeArg : Callee->args()) {
			if (calleeArg.getType() != Args[j]->getType())
				return IRError("Parameter type mismatch");
			j++;
		}

		return Builder->CreateCall(Callee, Args, "call");
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
		return Builder->CreateRet(returnVal);
	}
	case node_type::BreakExpression: {
		return Builder->CreateRetVoid();
	}

	default:
		return ConstantFP::get(Type::getDoubleTy(*TheContext), 1.0);
		break;
	}

}

