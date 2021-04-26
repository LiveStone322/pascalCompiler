#pragma once
#include "stdafx.h"

namespace comp
{
	class Node;

	class IdentTable
	{
	public:
		string name;
		int tableIndex;
		IdentTable *outer;
		vector<IdentTable *> inner;
		std::map<std::string, comp::Node *> dict;

		IdentTable();
		IdentTable(string name);
		void initFunctions();
		bool add(std::string key, Node *val);
		bool addInnerTable(IdentTable *innerTable);
		IdentTable *findInnerTable(string name);
		Node *lookup(std::string key);
		Node *lookupInScope(std::string key);
	};
}