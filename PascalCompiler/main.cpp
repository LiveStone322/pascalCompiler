#include "syntaxer.h"
#include "semantizer.h"
#include "identTable.h"
#include <time.h>
#include <fstream>
#pragma warning(disable : 4996)

void printSyntaxErrors(comp::Syntaxer* parser){
	if (parser->errList.size() > 0)
	{
		for (auto err : parser->errList)
		{
			err.errorPrint();
		}
	}
}

void PrintLine(int length)
{
	std::cout << endl;
	for (int i = 0; i < length; i++)
		std::cout << "_";
	std::cout << endl << endl;
}

void printPhase(string phaseName)
{
	PrintLine(phaseName.size() + 4);
	cout << "| " << phaseName << " |";
	PrintLine(phaseName.size() + 4);
}

void compile(string src)
{
	printPhase("--COMPILING " + src + "--");

	printPhase("Lexer");

	comp::Lexer lexer;
	lexer.readFile(src);
	lexer.scan();
	lexer.print();


	printPhase("Syntaxer");

	comp::IdentTable* symStack = new comp::IdentTable();
	comp::Syntaxer parser(&lexer);
	parser.parseProgram();
	printSyntaxErrors(&parser);
	parser.root->print();

	printPhase("Semantic");

	comp::semantizer semanticAnalyzer;
	parser.root->accept(&semanticAnalyzer);
	semanticAnalyzer.printTable(semanticAnalyzer.symTable);

	printPhase("Done");
}

int main(int argc, char* argv[])
{
	for (char i = '1'; i <= '3'; i++)
	{
		compile(string(1, i) + ".pas");
	}

	std::cout << "press Enter to exit." << endl;
	getchar();
	return 0;
}