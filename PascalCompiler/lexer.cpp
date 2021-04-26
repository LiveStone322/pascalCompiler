#include "lexer.h"
#include "errors.h"
#include "models.h"
#pragma warning(disable : 4996)

namespace comp
{

    bool isKeyword(string lex)
    {
        for (auto i : comp::keywords)
        {
            if (i.first.compare(lex) == 0)
                return true;
        }
        return false;
    }

    bool isDelimeter(char ch)
    {
        for (auto i : comp::delimeters)
        {
            if (i == ch)
                return true;
        }
        return false;
    }

    bool isWhitespace(char ch)
    {
        if (ch == '\n' || ch == '\r' || ch == '\t' || ch == ' ')
            return true;
        else
            return false;
    }
    bool isLetter(char ch)
    {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
    }
    bool isNum(char ch)
    {
        return ch >= '0' && ch <= '9';
    }
    bool isInteger(string num, int& val) {
        try {
            val = stoi(num);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    bool isDouble(string num, double& val) {
        try {
            val = stod(num);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }
    bool isLetterOrNum(char ch)
    {
        return isLetter(ch) || isNum(ch);
    }
    bool isEndLIne(char ch)
    {
        return ch == '\n';
    }

    /*
     * Lexer
     */

    Lexer::Lexer()
    {
        lineIndex = 1;
        curIndex = 0;
        state = comp::State::_START;
        tokens.clear();
    }

    void Lexer::readFile(string path)
    {
        //открываем файл на чтение
        FILE* f = fopen(path.c_str(), "r");
        fpos_t pos;

        //не такого файла
        if (f == NULL)
            throw new exception();

        //считываем весь файл
        fseek(f, 0, SEEK_END);
        fgetpos(f, &pos);
        fseek(f, 0, SEEK_SET);
        char* multiBuffer = new char[(size_t)pos + 1];
        fread(multiBuffer, 1, (size_t)pos, f);
        multiBuffer[(size_t)pos] = '\0';
        fclose(f);

        //заносим исходный текст программы в переменную
        code = multiBuffer;
        delete[] multiBuffer;
    }

    char Lexer::nextChar()
    {
        if (curIndex < code.length())
        {
            curChar = code[curIndex++];
            return curChar;
        }
        else
            return '\0';
    }

    bool Lexer::readChar(char c)
    {
        nextChar();
        if (curChar != c)
            return false;
        curChar = ' ';
        return true;
    }

    void Lexer::addToken(Token token)
    {
        tokens.push_back(token);
    }

    void Lexer::scan()
    {
        Token* curToken = new Token;
        string lex = "";

        while (nextChar() != '\0')
        {
            bool lexemEndFlag = false;
            switch (state)
            {
            case comp::State::_START:
                if (isWhitespace(curChar))
                {
                    if (isEndLIne(curChar))
                        lineIndex++;
                    continue;
                }
                if (curChar == '\"' || curChar == '\'')
                {
                    state = comp::State::_STRING;
                    continue;
                }
                if (isNum(curChar))
                {
                    state = comp::State::_INT;
                }
                else if (isLetter(curChar))
                {
                    state = comp::State::_IDENT;
                }
                else
                {
                    switch (curChar)
                    {
                    case ';':
                        curToken->tag = comp::Tag::SEMI;
                        lexemEndFlag = true;
                        break;

                    case '(':
                        curToken->tag = comp::Tag::OPEN_BRACKET;
                        lexemEndFlag = true;
                        break;

                    case ')':
                        curToken->tag = comp::Tag::CLOSE_BRACKET;
                        lexemEndFlag = true;
                        break;

                    case ',':
                        curToken->tag = comp::Tag::COMMA;
                        lexemEndFlag = true;
                        break;

                    case '.':
                        curToken->tag = comp::Tag::FINISH;
                        lexemEndFlag = true;
                        break;

                    case ':':
                        switch (code.at(curIndex))
                        {
                        case '=':
                            lex.append(&curChar);
                            curToken->tag = comp::Tag::BIND;
                            lexemEndFlag = true;
                            nextChar();
                            break;
                        default:
                            curToken->tag = comp::Tag::COLON;
                            lexemEndFlag = true;
                            break;
                        }
                        break;
                    case '&':
                        curToken->tag = comp::Tag::CONCAT;
                        lexemEndFlag = true;
                        break;
                    case '+':
                        curToken->tag = comp::Tag::ADD;
                        lexemEndFlag = true;
                        break;
                    case '-':
                        curToken->tag = comp::Tag::SUB;
                        lexemEndFlag = true;
                        break;
                    case '*':
                        curToken->tag = comp::Tag::MUL;
                        lexemEndFlag = true;
                        break;
                    case '/':
                        curToken->tag = comp::Tag::DIV;
                        lexemEndFlag = true;
                        break;
                    case '<':
                        switch (code.at(curIndex))
                        {
                        case '=':
                            lex.append(&curChar);
                            curToken->tag = comp::Tag::LE;
                            lexemEndFlag = true;
                            nextChar();
                            break;
                        case '>':
                            lex.append(&curChar);
                            curToken->tag = comp::Tag::UNEQ;
                            lexemEndFlag = true;
                            nextChar();
                            break;
                        default:
                            curToken->tag = comp::Tag::LT;
                            lexemEndFlag = true;
                            break;
                        }
                        break;
                    case '>':
                        switch (code.at(curIndex))
                        {
                        case '=':
                            lex.append(&curChar);
                            curToken->tag = comp::Tag::GE;
                            lexemEndFlag = true;
                            nextChar();
                            break;
                        default:
                            lexemEndFlag = true;
                            curToken->tag = comp::Tag::GT;
                            break;
                        }
                        break;
                    case '=':
                        curToken->tag = comp::Tag::EQ;
                        lexemEndFlag = true;
                        break;
                    }
                }
                lex.append(&curChar);
                break;
            case comp::State::_INT:
                if (curChar == '.')
                {
                    lex.append(&curChar);
                    state = comp::State::_FLOAT;
                }
                else if (isNum(curChar))
                {
                    lex.append(&curChar);
                }
                else if (isDelimeter(curChar) || isWhitespace(curChar))
                {
                    lexemEndFlag = true;
                }
                break;
            case comp::State::_FLOAT:
                if (curChar == '.')
                {
                    cout << "line " << lineIndex << ": " << humanReadableErrors.find(BAD_CONSTANT)->second.c_str() << endl;
                    lexemEndFlag = true;
                    skipLexeme();
                    break;
                }
                if (isNum(curChar))
                {
                    lex.append(&curChar);
                }
                else if (isDelimeter(curChar) || isWhitespace(curChar))
                {
                    lexemEndFlag = true;
                }
                break;
            case comp::State::_STRING:
                if (curChar != '\"' && curChar != '\'')
                {
                    lex.append(&curChar);
                    break;
                }
                lexemEndFlag = true;
                break;
            case comp::State::_IDENT:
                if (isLetterOrNum(curChar))
                {
                    lex.append(&curChar);
                }
                else if (isDelimeter(curChar) || isWhitespace(curChar))
                {
                    lexemEndFlag = true;
                }
                break;

            default:
                break;
            }
            if (lexemEndFlag)
            {
                switch (state)
                {
                case comp::State::_INT:

                    int valI;
                    curToken->tag = comp::Tag::INT;
                    curIndex--;
                    break;
                case comp::State::_FLOAT:

                    double valD;
                    curToken->tag = comp::Tag::FLOAT;
                    curIndex--;
                    break;
                case comp::State::_STRING:
                    curToken->tag = comp::Tag::STRING;
                    break;
                case comp::State::_IDENT:
                    if (isKeyword(lex))
                    {
                        curToken->tag = comp::keywords.at(lex);
                    }
                    else
                        curToken->tag = comp::Tag::IDENT;
                    curIndex--;
                    break;
                default:
                    break;
                }
                state = comp::State::_START;
                curToken->value = lex;
                curToken->lineIndex = lineIndex;
                lex = "";
                addToken(*curToken);
            }
        }
        delete curToken;
    }

    void Lexer::skipLexeme() {
        do {
            nextChar();
        } while (!(isDelimeter(curChar) || isWhitespace(curChar)));
    }

    void Lexer::print()
    {
        for (auto i : tokens)
        {
            cout << "|\tline: " << i.lineIndex << "\t|\ttag: " << humanReadableTags[i.tag] << "\t|\tlexeme: " << i.value << "\t|" << endl;
        }
    }
}
