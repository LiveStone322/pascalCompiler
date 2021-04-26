#pragma once
#include "models.h"
#include "stdafx.h"
#include <vector>

namespace comp
{
	class Lexer
	{
	private:
		char nextChar();
		bool readChar(char c);

	public:
		comp::State state;
		int lineIndex;
		string code;
		u_int curIndex;
		char curChar;
		vector<Token> tokens;

		void readFile(string path);
		void scan();
		void skipLexeme();
		void addToken(Token token);
		void print();
		Lexer();
	};
}