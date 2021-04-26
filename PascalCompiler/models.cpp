#include "models.h";
using namespace std;

namespace comp {
	bool Token::operator=(const Token& t)
	{
		tag = t.tag;
		value = t.value;
		lineIndex = t.lineIndex;
		return true;
	}

	Token::Token()
	{
	}

	Token::Token(comp::Tag tag, string value, int lineIndex)
	{
		this->tag = tag;
		this->value = value;
		this->lineIndex = lineIndex;
	}

	Token::Token(const Token& t)
	{
		tag = t.tag;
		value = t.value;
		lineIndex = t.lineIndex;
	}
}