#pragma once
#include <iostream>
#include <string>
#include "models.h"
using namespace std;
using namespace comp;

class Error
{
public:
	int errCode;
	ErrorType errType;
	Token errToken;
	int errLine;
	Error() {}
	Error(int errCode, ErrorType errType, Token errToken)
	{
		this->errCode = errCode;
		this->errType = errType;
		this->errToken = errToken;
		errLine = errToken.lineIndex;
	}
	void errorPrint();
	~Error() {}
};