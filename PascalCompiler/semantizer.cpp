#include "semantizer.h"
#include "pascalSemantics.h"
#include "syntaxer.h"
#include "identTable.h"
#include "Declaration.h"
#include "errors.h"
#include "models.h"
using namespace comp;

semantizer::semantizer()
{
	this->symTable = new IdentTable("global");
	currentTable = symTable;
}

semantizer::~semantizer() {}
void semantizer::visit(Node *node)
{
	currentTable->add(node->value.value, node);
	for (auto item : node->list)
	{
		item->accept(this);
	}
}
void semantizer::visit(Statement* node)
{
	for (auto item : node->list)
	{
		if (item) item->accept(this);
	}
}
void semantizer::visit(Expression* node) {}
void semantizer::visit(ComparisonExp* node) {}
void semantizer::visit(AssignStmt* node) {}
void semantizer::visit(FuncCall* node) {}
void semantizer::visit(IfStmt* node)
{
	IdentTable *symTable1 = new IdentTable();
	currentTable->addInnerTable(symTable1);
	symTable1->outer = currentTable;
	currentTable = symTable1;
	node->thenBody->accept(this);
	if (node->haveElse)
	{
		node->elseBody->accept(this);
	}
	currentTable = symTable1->outer;
}
void semantizer::visit(ElseStmt* node) {}
void semantizer::visit(CaseStmt* node) {}
void semantizer::visit(FunctionStmt* node)
{
	currentTable->add(node->funcDecl->name, node);
	IdentTable *symTable1 = new IdentTable(node->funcDecl->name);
	currentTable->addInnerTable(symTable1);
	symTable1->outer = currentTable;
	currentTable = symTable1;
	for (auto item : node->funcDecl->vars)
	{
		Expression *newnode=new Expression;
		newnode->value = item->identity;
		symTable1->add(item->name, newnode);
	}
	if (node->list.size() > 0)
	{
		node->list[0]->accept(this);
	}
	node->body->accept(this);
	currentTable = symTable1->outer;
}
void semantizer::visit(VariableStmt* node)
{
	if (node->varDeclare == NULL)
	{
		for (auto item : node->varRoot)
		{
			if (currentTable->lookup(item->varDeclare->name) != NULL)
			{
				Error err((int)REDIFINED_VAR, ErrorType::Semantic, item->varDeclare->identity);
				errList.push_back(err);
			}
			else
			{
				currentTable->add(item->varDeclare->name, item.get());
			}
		}
	}
}
void semantizer::visit(TypeStmt* node)
{
	if (node->typeRoot.size() > 0)
	{
		for (auto item : node->typeRoot)
		{
			currentTable->add(item->typeDeclare->name, item.get());
			if (item->varStmts.size()>0)
			{
				for (auto j : item->varStmts)
				{
					currentTable->add(item->typeDeclare->name + "_" + j->varDeclare->name, j.get());
				}
			}
		}
	}
}
void semantizer::visit(ConstantStmt* node)
{
	if (node->constDeclare == NULL && node->constRoot.size()>0)
	{
		for (auto item : node->constRoot)
		{
			if (currentTable->lookup(item->constDeclare->name) != NULL)
			{
				Error err((int)REDIFINED_VAR, ErrorType::Semantic, node->value);
				errList.push_back(err);
			}
			else
			{
				currentTable->add(item->constDeclare->name, item.get());
			}
		}
	}
}
void semantizer::printTable(IdentTable* table)
{
	for (auto item : table->dict)
	{
		cout << "|\t " << item.first << "\t|\t" << humanReadableTags[item.second->value.tag] << "\t|" << endl;
	}
}