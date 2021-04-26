#pragma once
#include "semantizerBase.h"
#include "errors.h"

namespace comp
{
	class IdentTable;
	class semantizer : public semantizerBase
	{
	public:
		IdentTable *symTable;
		IdentTable *currentTable;
		std::vector<Error> errList;
		semantizer();
		~semantizer();
		void visit(Node *node);
		void visit(Statement* node);
		void visit(Expression* node);
		void visit(ComparisonExp* node);
		void visit(AssignStmt* node);
		void visit(FuncCall* node);
		void visit(IfStmt* node);
		void visit(ElseStmt* node);
		void visit(CaseStmt* node);
		void visit(FunctionStmt* node);
		void visit(VariableStmt* node);
		void visit(TypeStmt* node);
		void visit(ConstantStmt* node);
		void printTable(IdentTable* t);
	};
}

