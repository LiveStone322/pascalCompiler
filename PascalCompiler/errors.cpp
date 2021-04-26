#include "errors.h"

void Error::errorPrint()
{
	std::cout << "|ERROR!\t" << errToken.value << "\t|\t" << humanReadableErrors.at(errCode) << "\t|\tin line " << errLine << " |" << std::endl;
}