#pragma once
#include "stdafx.h"
namespace comp
{
	class Node;
	class Statement;
	class Expression;
	class ComparisonExp;
	class AssignStmt;
	class FuncCall;
	class IfStmt;
	class ElseStmt;
	class WhileStmt;
	class RepeatStmt;
	class ForStmt;
	class CaseStmt;
	class FunctionStmt;
	class VariableStmt;
	class TypeStmt;
	class ConstantStmt;
	class semantizerBase
	{
	public:
		semantizerBase();
		~semantizerBase();
		virtual void visit(Node *node) = 0;
		virtual void visit(Statement* node) = 0;
		virtual void visit(Expression* node) = 0;
		virtual void visit(ComparisonExp* node) = 0;
		virtual void visit(AssignStmt* node) = 0;
		virtual void visit(FuncCall* node) = 0;
		virtual void visit(IfStmt* node) = 0;
		virtual void visit(ElseStmt* node) = 0;
		virtual void visit(FunctionStmt* node) = 0;
		virtual void visit(VariableStmt* node) = 0;
		virtual void visit(TypeStmt* node) = 0;
		virtual void visit(ConstantStmt* node) = 0;
	};
}


