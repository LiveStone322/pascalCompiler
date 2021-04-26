#include "syntaxer.h"
#include "Declaration.h"
#include "models.h"

namespace comp
{
	void Node::print()
	{
		for (auto node : list)
		{
			std::cout <<"|\t" <<"type:" << humanReadableTags[node->value.tag]<<"\t|\t"
				<<"node"<<"\t|" << endl;
			node->print();
		}
	}

	void Statement::print()
	{
		std::cout << "|\t" << "type:" << humanReadableTags[this->value.tag] << "\t|\t" 
			<< "Statements" << "\t|" << endl;
		for (auto item : list)
		{
			if (item) item->print();
		}
	}

	void Expression::print()
	{
		std::cout << "|\t" << "type:" << humanReadableTags[this->value.tag]
			<< "\t|\t" << this->value.value << "\t|"  << endl;
		for (auto item : list)
		{
			item->print();
		}
	}

	void AssignStmt::print()
	{
		cout << "| Assignment |" << endl;
		left->print();
		right->print();
	}

	void ComparisonExp::print()
	{
		cout << endl;
		cout << "| Comparision |" << endl;
		left->print();
		cout << "|\t" << "type:" << humanReadableTags[op.tag] << "\t|\t" << op.value << "|\t" << endl;
		right->print();
		cout << endl;
	}

	void IfStmt::print()
	{
		cout << endl;
		cout << "| If statement |" << endl;
		condition->print();
		this->thenBody->print();
		if (this->haveElse)
		{
			this->elseBody->print();
		}
		cout << endl;
	}

	void FuncCall::print()
	{
		cout << "| Function call |" << endl;
		cout << "|\t" <<this->funcName<<"( ";
		string addition = this->constants.size() > 1 ? " " : "";
		for (auto item : this->constants)
		{
			cout << item.value << addition;
		}
		cout <<" )"<< "\t|" << endl;
	}

	void FunctionStmt::print()
	{
		cout << this->funcDecl->name << " " << this->funcDecl->returnType << endl;
		this->body->print();
	}

	void ConstantStmt::print()
	{
		if (this->constRoot.size() == 0 || this->constDeclare != NULL)
		{
			cout << "|\t" << "Constant:" << "\t|\t" << this->constDeclare->name << "\t|" << endl;
		}
		if (this->constRoot.size() > 0)
		{
			for (auto item : this->constRoot)
			{
				item->print();
			}
		}
	}

	void TypeStmt::print()
	{
		if (this->typeRoot.size() == 0 || this->typeDeclare != NULL)
		{
			cout << "|\t" << "Constant:" << "\t|\t" << this->typeDeclare->name << "\t|" << endl;
		}
		if (this->typeRoot.size() > 0)
		{
			for (auto item : this->typeRoot)
			{
				item->print();
			}
		}
	}
	void VariableStmt::print()
	{
		if (this->varRoot.size() == 0 || this->varDeclare != NULL)
		{
			cout << "|\t" << "Constant:" << "\t|\t" << this->varDeclare->name << "\t|\t" << endl;
		}
		if (this->varRoot.size() > 0)
		{
			for (auto item : this->varRoot)
			{
				item->print();
			}
		}
	}
	bool Node::operator=(Node &node)
	{
		this->value = node.value;
		this->parent = node.parent;
		list.resize(node.list.size());
		std::copy(node.list.begin(), node.list.end(), std::back_inserter(list));
		return 1;
	}

	/*
	 *	Parser
	 */
	bool Syntaxer::advance(Tag t)
	{
		if ((it + 1)->tag == t)
		{
			return true;
		}
		else
			return false;
	}

	bool Syntaxer::find(Tag t)
	{
		if (it->tag == t)
		{
			return true;
		}
		else
			return false;
	}

	bool Syntaxer::accept(Tag t)
	{
		if (it->tag == t)
			return true;
		else
		{
			Error err(int(SyntaxerError::SYNTAX_ERROR), ErrorType::Syntax, *it);
			errList.push_back(err);
			return false;
		}
	}

	void Syntaxer::parseProgram()
	{
		if (it->tag == PROGRAM)
		{
			it++;
			shared_ptr<Node> beginNode = make_shared<Node>();
			beginNode->value = *it;
			root = beginNode;
			currNode = root;
			it++;
			accept(SEMI);
			parseBlock();
		}
		else
		{
			Error err(SyntaxerError::MISSING_FINISH_SIGN, ErrorType::Syntax, *it);
			errList.push_back(err);
		}
		if (it->tag != FINISH)
		{
			Error err(int(SyntaxerError::MISSING_FINISH_SIGN), ErrorType::Syntax, *it);
			errList.push_back(err);
		}
	}

	void Syntaxer::parseBlock()
	{
		++it;
		switch (it->tag)
		{
			case CONSTANT:
				parseConstant();
				parseBlock();
				break;
			case VAR:
				parseVariables();
				parseBlock();
				break;
			case TYPE:
				parseType();
				parseBlock();
				break;
			case FUNCTION:
				parseFunction();
				parseBlock();
				break;
			case PROCEDURE:
				parseProcedure();
				parseBlock();
				break;
			case BEGIN:
				{
					shared_ptr<Statement> block = 
						parseStmtList(currNode == root ? FINISH : END);
					if (currNode != root)
					{
						(static_pointer_cast<FunctionStmt>(currNode))->body=block;
					}
					else
					{
						block->value.value = "MAIN_BLOCK";
						currNode->addNode(block);
					}
					bool tmp=it->tag == FINISH || it->tag == END;
					if (it->tag == END)
					{
						it++;
						accept(SEMI);
					}
				}
				break;
		}
	}
	shared_ptr<Statement> Syntaxer::parseProcedureBase(bool isFunction)
	{
		shared_ptr<FunctionStmt> funcStmt = make_shared<FunctionStmt>();
		shared_ptr<FunctionDecl> funcDecl = make_shared<FunctionDecl>();
		funcStmt->funcDecl = funcDecl;
		currNode->addNode(funcStmt);
		shared_ptr<Node> tmpNode = currNode;
		currNode = funcStmt;
		it++;
		if (it->tag == IDENT)
		{
			funcDecl->name = it->value;
		}
		it++;
		accept(Tag::OPEN_BRACKET);
		it++;
		while (it->tag != Tag::CLOSE_BRACKET)
		{
			vector<std::string> args;
			while (!args.empty())args.pop_back();
			auto parseArgs = [&](){
				while (it->tag != Tag::COLON)
				{
					args.push_back(it->value);
					it++;
					if (it->tag == Tag::COMMA)
					{
						it++;
					}
				}
				it++;
				for (auto val : args)
				{
					shared_ptr<VariableDecl> varDecl = make_shared<VariableDecl>();
					varDecl->name = val;
					varDecl->type = DeclaredType::Variable;
					varDecl->identity = *it;
					funcDecl->vars.push_back(varDecl);
				}
			};

			if (it->tag == VAR)
			{
				it++;
				parseArgs();
			}
			else
			{
				parseArgs();
			}
			it++;
			if (find(SEMI))
			{
				it++;
			}
			else
			{
				break;
			}
		}
		it++;
		accept(Tag::COLON);
		it++;
		funcDecl->type = DeclaredType::Procedure;
		if (isFunction)
		{
			funcDecl->type = DeclaredType::Function;
			funcDecl->returnType = it->value;
			it++;
		}
		accept(SEMI);
		if (!advance(FORWORD))
		{
			parseBlock();
			currNode = tmpNode;
			return funcStmt;
		}
		else
		{
			it++;
			accept(FORWORD);
			it++;
			accept(SEMI);
			return funcStmt;
		}
	}

	shared_ptr<Statement> Syntaxer::parseProcedure()
	{
		return parseProcedureBase(false);
	}
	shared_ptr<Statement> Syntaxer::parseFunction()
	{
		return parseProcedureBase(true);
	}

	shared_ptr<VariableStmt> Syntaxer::parseVariables()
	{
		shared_ptr<VariableStmt> rootVal = make_shared<VariableStmt>();
		rootVal->varDeclare = NULL;
		while (advance(IDENT))
		{
			auto varDecl = parseVariable();
			auto varStmt = make_shared<VariableStmt>();
			varStmt->varDeclare = varDecl;
			varStmt->value.value = "VariableStmt";
			varStmt->value.tag = Tag::VAR;
			rootVal->varRoot.push_back(varStmt);
		}
		currNode->addNode(rootVal);
		return rootVal;
	}

	shared_ptr<VariableDecl> Syntaxer::parseVariable()
	{
		auto varDecl= make_shared<VariableDecl>();
		it++;
		varDecl->name = it->value;
		varDecl->type = DeclaredType::Variable;
		it++;
		accept(Tag::COLON);
		it++;
		varDecl->identity = *it;
		it++;
		accept(SEMI);
		return varDecl;
	}

	shared_ptr<TypeStmt> Syntaxer::parseType()
	{
		it++;
		auto typeRootVal = make_shared<TypeStmt>();
		typeRootVal->typeDeclare = NULL;
		while (it->tag == IDENT)
		{
			auto typeDecl=make_shared<TypeDecl>();

			auto typeStmt = make_shared<TypeStmt>();
			typeStmt->typeDeclare = typeDecl;
			typeStmt->value.value = "TypeStmt";
			typeStmt->value.tag = Tag::TYPE;
			typeDecl->name = it->value;
			typeDecl->type = DeclaredType::Record;
			
			it++;
			accept(EQ);
			it++;
			switch (it->tag)
			{
				case RECORD:
					while ((it + 1)->tag != END)
					{
						auto record1=parseVariable();
						auto varStmt = make_shared<VariableStmt>();
						varStmt->varDeclare = record1;
						varStmt->value.value = "VariableStmt";
						varStmt->value.tag = Tag::VAR;
						typeDecl->vars.push_back(record1);
						typeStmt->varStmts.push_back(varStmt);
					}
					it++;
					it++;
					accept(SEMI);
					break;
				default:
					break;
			}
			typeRootVal->typeRoot.push_back(typeStmt);
		}
		currNode->addNode(typeRootVal);
		return typeRootVal;
	}

	shared_ptr<ConstantStmt> Syntaxer::parseConstant()
	{
		shared_ptr<ConstantStmt> rootVal = make_shared<ConstantStmt>();
		rootVal->constDeclare = NULL;
		it++;
		while (it->tag == IDENT)
		{
			auto constDecl= make_shared<ConstantDecl>();
			auto constStmt = make_shared<ConstantStmt>();
			constStmt->constDeclare = constDecl;
			constStmt->value.value = "ConstantStmt";
			constStmt->value.tag = Tag::CONSTANT;
			constDecl->name = it->value;
			constDecl->type = DeclaredType::Constant;
			it++;
			accept(BIND);
			it++;
			constDecl->value = *it;
			it++;
			accept(SEMI);
			++it;
			rootVal->constRoot.push_back(constStmt);
		}
		currNode->addNode(rootVal);
		it--;
		return rootVal;
	}

	shared_ptr<AssignStmt> Syntaxer::parseAssign()
	{
		auto assign = make_shared<AssignStmt>();
		auto exp = make_shared<Expression>();
		exp->value = *it;
		assign->left = exp;
		it++;
		accept(BIND);
		assign->value = *it;
		it++;
		assign->right = parseExpression();
		if (!(find(SEMI) || find(TO)))
		{
			Error err(int(SyntaxerError::SYNTAX_ERROR), ErrorType::Syntax, *it);
			errList.push_back(err);
		}

		return assign;
	}

	shared_ptr<ComparisonExp> Syntaxer::parseComparison()
	{
		auto compareStmt = shared_ptr<ComparisonExp>();
		auto exp = parseExpression();
		compareStmt->left = exp;
		it++;
		compareStmt->op = *it;
		it++;
		compareStmt->right = parseExpression();
		return compareStmt;
	}

	shared_ptr<Expression> Syntaxer::parseExpression()
	{
		auto rootExp = parseExpression1();
		it++;
		
		while (it->tag == OR || it->tag == AND)
		{
			auto opExp = make_shared<Expression>();
			opExp->addNode(rootExp);
			opExp->value = *it;
			it++;
			opExp->addNode(parseExpression1());
			rootExp = opExp;
		}
		return rootExp;
	}

	shared_ptr<Expression> Syntaxer::parseExpression1()
	{
		Tag tags[] = { EQ, UNEQ, LT, GT, LE, GE };

		auto isRealOp = [&](Tag t){
			for (auto i : tags)
			{
				if (t == i)
				{
					return true;
				}
			}
			return false;
		};
		auto rootExp = parseTerm();
		it++;
		while (it->tag == ADD || it->tag == SUB || isRealOp(it->tag))
		{
			auto opExp = make_shared<Expression>();
			opExp->addNode(rootExp);
			opExp->value = *it;
			it++;
			opExp->addNode(parseTerm());
			rootExp = opExp;
			it++;
		}
		if (!(it->tag == ADD || it->tag == SUB || isRealOp(it->tag)))
		{
			it--;
		}
		return rootExp;
	}

	shared_ptr<Expression> Syntaxer::parseTerm()
	{
		auto rootExp = parseFactor();
		it++;
		
		while (it->tag == MUL || it->tag == DIV )
		{
			auto opExp = make_shared<Expression>();
			opExp->value = *it;
			opExp->addNode(rootExp);
			it++;
			opExp->addNode(parseTerm());
			rootExp = opExp;
			it++;
		}
		if (!(it->tag == MUL || it->tag == DIV))
		{
			it--;
		}
		return rootExp;
	}

	shared_ptr<Expression> Syntaxer::parseFactor()
	{
		if (it->tag == IDENT || 
			it->tag == INT || 
			it->tag == FLOAT ||
			it->tag == STRING)
		{
			auto exp = make_shared<Expression>();
			exp->value = *it;
			return exp;
		}
		else if (it->tag == Tag::OPEN_BRACKET)
		{
			auto exp = parseExpression();
			accept(Tag::CLOSE_BRACKET);
			return exp;
		}
	}

	shared_ptr<Statement> Syntaxer::parseStmtList(Tag terminator)
	{
		auto tmpNode = currNode;
		shared_ptr<Statement> stList = make_shared<Statement>();
		currNode = stList;

		stList->value = *it;
		it++;
		while (it->tag != terminator && it->tag != FINISH)
		{
			if (it->tag == END && terminator != END)
			{
				shared_ptr<Statement> st1 = make_shared<Statement>();
				st1->value = *it;
				stList->addNode(st1);
				it++;
			}
			shared_ptr<Statement> st = parseStatement();
			stList->addNode(st);
			if (!it->tag == FINISH) it++;
		}

		currNode = tmpNode;
		return stList;
	}

	shared_ptr<Statement> Syntaxer::parseStatement()
	{
		Tag tags[] = { EQ, UNEQ, LT, GT, LE, GE };

		auto isRealOp = [&](Tag t){
			for (auto i : tags)
			{
				if (t == i)
				{
					return true;
				}
			}
			return false;
		};
		switch (it->tag)
		{
			case IDENT:
				if (advance(BIND))
				{
					shared_ptr<AssignStmt> assign = parseAssign();
					return assign;
				}
				else if (advance(Tag::OPEN_BRACKET))
				{
					shared_ptr<FuncCall> funcCall = parseFunctionCall();
					return funcCall;
				}
			case IF:
			{
				shared_ptr<Node> tmp = currNode;
				shared_ptr<IfStmt> ifstmt = static_pointer_cast<IfStmt>(parseIf());
				currNode = tmp;
				return ifstmt; 
			}
			default:
				return NULL;
		}
		return NULL;
	}

	shared_ptr<Statement> Syntaxer::parseIf()
	{
		shared_ptr<IfStmt> ifstmt = make_shared<IfStmt>();
		auto cond = parseExpression();
		ifstmt->condition = cond;
		accept(THEN);
		it++;
		if (accept(BEGIN))
		{
			currNode = ifstmt;
			shared_ptr<Statement> body = parseStmtList(END);
			ifstmt->thenBody=body;
			accept(END);
			it++;
			accept(SEMI);
		}
		else
		{
			shared_ptr<Statement> body1 = parseStatement();
			ifstmt->thenBody = body1;
		}
		ifstmt->haveElse = false;
		while (advance(ELSE))
		{
			ifstmt->haveElse = true;
			it++;
			shared_ptr<ElseStmt> elseStmt = static_pointer_cast<ElseStmt>(parseElse());
			ifstmt->elseBody = elseStmt;
		}
		return ifstmt;
	}

	shared_ptr<Statement> Syntaxer::parseElse()
	{
		shared_ptr<ElseStmt> elseStmt = make_shared<ElseStmt>();
		elseStmt->value = *it;
		it++;
		if (accept(IF))
		{
			shared_ptr<IfStmt> elseifnode = static_pointer_cast<IfStmt>(parseIf());
			elseStmt->addNode(elseifnode);
		}
		else if (accept(BEGIN))
		{
			shared_ptr<Statement> body = parseStmtList(END);
			elseStmt->body = body;
			accept(END);
			it++;
			accept(SEMI);
		}
		else
		{
			shared_ptr<Statement> body1 = parseStatement();
			elseStmt->addNode(body1);
		}
		return elseStmt;
	}

	shared_ptr<FuncCall> Syntaxer::parseFunctionCall()
	{
		shared_ptr<FuncCall> funcCall = make_shared<FuncCall>();
		funcCall->funcName = it->value;
		++it;
		accept(Tag::OPEN_BRACKET);
		++it;
		Tag arg[] = { Tag::INT, Tag::CHAR, Tag::FLOAT, Tag::BOOL, Tag::STRING, Tag::IDENT };
		auto isArg = [&](Tag tag){
			for (auto i : arg)
			{
				if (i == tag)
				{
					return true;
				}
			}
			return false;
		};
		while(isArg(it->tag))
		{
			funcCall->constants.push_back(*it);
			it++;
			if (it->tag != Tag::CLOSE_BRACKET)
			{
				accept(Tag::COMMA);
				it++;
			}
			else
				break;
		}
		accept(Tag::CLOSE_BRACKET);
		it++;
		accept(SEMI);
		return funcCall;
	}
}