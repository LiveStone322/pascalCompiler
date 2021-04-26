#pragma once
#include "stdafx.h"
#include "lexer.h"
namespace comp
{

	enum class DeclaredType
	{
		Constant, Enumeration,
		Type, Variable, Record,
		Program, Procedure, Function,
		Undefined, 
		INT, FLOAT, STRING, USERTYPE 
	};

	class Declaration
	{
	public:
		DeclaredType type;
		string name;
	};

	class ConstantDecl :public Declaration
	{
	public:
		Token value;
	};

	class VariableDecl :public Declaration
	{
	public:
		Token identity;

	};

	class TypeDecl :public Declaration 
	{
	public:
		vector<shared_ptr<VariableDecl> > vars;
	};

	class FunctionDecl :public Declaration
	{
	public:
		std::vector<shared_ptr<VariableDecl> > vars;
		std::string returnType;
	};
}