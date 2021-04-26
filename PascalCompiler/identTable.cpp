#include "identTable.h"
#include "pascalSemantics.h"
namespace comp
{
	IdentTable::IdentTable()
	{
		tableIndex = 0;
		this->name = "";
		outer = NULL;
	}
	IdentTable::IdentTable(string name)
	{
		tableIndex = 0;
		this->name = name;
		outer = NULL;
	}
	void IdentTable::initFunctions()
	{
	}
	bool IdentTable::addInnerTable(IdentTable *innerTable)
	{
		if (innerTable->name == "")
		{
			innerTable->name = to_string(tableIndex);
		}
		tableIndex++;
		inner.push_back(innerTable);
		return true;
	}
	IdentTable *IdentTable::findInnerTable(string name)
	{
		if (inner.size() > 0)
		{
			for (auto item : inner)
			{
				if (item->name == name)
				{
					return item;
				}
			}
		}
	}
	bool IdentTable::add(std::string key, Node *val)
	{
		if (dict.find(key) == dict.end())
		{
			dict.insert(std::pair<string, Node *>(key, val));
			return true;
		}
		return false;
	}
	Node *IdentTable::lookup(std::string key)
	{
		IdentTable *env = this;
		while (env != NULL)
		{
			if (env->dict.find(key) != env->dict.end())
			{
				return env->dict[key];
			}
			if (env->outer != NULL)
			{
				env = env->outer;
			}
			else
				break;
		}
		return NULL;
	}
	Node *IdentTable::lookupInScope(std::string key)
	{
		if (dict.find(key) != dict.end())
		{
			return dict[key];
		}
		return NULL;
	}
}