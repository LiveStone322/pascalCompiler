#pragma once
#include "pascalSemantics.h"
#include "errors.h"
namespace comp
{
	class Syntaxer
	{
	public:
		std::vector<Token>::iterator it;
		std::vector<Error> errList;
		
		shared_ptr<Node> root;
	protected:
		shared_ptr<Node> currNode;
		Lexer *lex;
	public:
		Syntaxer(Lexer *l)
		{
			lex = l;
			it = lex->tokens.begin();
			auto test = lex->tokens.end();
		}
		bool find(Tag t);
		bool advance(Tag t);
		bool accept(Tag t);
		void parseProgram();
		void parseBlock();
		shared_ptr<VariableDecl> parseVariable();
		shared_ptr<VariableStmt> parseVariables();
		shared_ptr<TypeStmt> parseType();
		shared_ptr<ConstantStmt> parseConstant();
		shared_ptr<AssignStmt> parseAssign();
		shared_ptr<ComparisonExp> parseComparison();
		shared_ptr<Expression> parseExpression();
		shared_ptr<Expression> parseExpression1();
		shared_ptr<Expression> parseTerm();
		shared_ptr<Expression> parseFactor();
		shared_ptr<Statement> parseStmtList(Tag teminator);
		shared_ptr<Statement> parseStatement();
		shared_ptr<Statement> parseIf();
		shared_ptr<Statement> parseElse();
		shared_ptr<FuncCall> parseFunctionCall();
		shared_ptr<Statement> parseFunction();
		shared_ptr<Statement> parseProcedure();
		shared_ptr<Statement> parseProcedureBase(bool isFunction);
	};
}