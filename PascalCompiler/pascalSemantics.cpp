#include "pascalSemantics.h"
#include "semantizerBase.h"

namespace comp
{
	void Node::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void Statement::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void Expression::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void AssignStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void ComparisonExp::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void FuncCall::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void IfStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void ElseStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void FunctionStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void VariableStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void TypeStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
	void ConstantStmt::accept(semantizerBase *visitor)
	{
		visitor->visit(this);
	}
}