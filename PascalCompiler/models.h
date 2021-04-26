#pragma once
#include <string>
#include <map>
using namespace std;

namespace comp {
    enum Tag {
        FINISH,
        BLOCK,
        IF,
        THEN,
        ELSE,
        WHILE,
        FUNCTION,
        PROCEDURE,
        BREAK,
        BEGIN,
        END,
        PROGRAM,
        CASE,
        OF,
        REPEAT,
        UNTIL,
        DO,
        VAR,
        TYPE,
        RECORD,
        CONSTANT,
        FOR,
        TO,
        DOWNTO,
        FORWORD,
        INT,
        CHAR,
        FLOAT,
        BOOL,
        STRING,
        IDENT,
        GT,
        LT,
        EQ,
        UNEQ,
        GE,
        LE,
        AND,
        NOT,
        OR,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        COMMA,
        COLON,
        CONCAT,
        SEMI,
        ADD,
        SUB,
        MUL,
        DIV,
        BIND
    };

    const string humanReadableTags[] = {
        "FINISH",
        "BLOCK",
        "IF",
        "THEN",
        "ELSE",
        "WHILE",
        "FUNCTION",
        "PROCEDURE",
        "BREAK",
        "BEGIN",
        "END",
        "PROGRAM",
        "CASE",
        "OF",
        "REPEAT",
        "UNTIL",
        "DO",
        "VAR",
        "TYPE",
        "RECORD",
        "CONSTANT",
        "FOR",
        "TO",
        "DOWNTO",
        "FORWORD",
        "INT",
        "CHAR",
        "FLOAT",
        "BOOL",
        "STRING",
        "IDENT",
        "GT",
        "LT",
        "EQ",
        "UNEQ",
        "GE",
        "LE",
        "AND",
        "NOT",
        "OR",
        "OPEN_BRACKET",
        "CLOSE_BRACKET",
        "COMMA",
        "COLON",
        "CONCAT",
        "SEMI",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "BIND",
    };

    enum ErrorType {
        Lexical,
        Syntax,
        Semantic
    };

    enum LexerError {
        BAD_LITERA = 0,
        BAD_CONSTANT = 1,
    };

    enum SyntaxerError {
        SYNTAX_ERROR = 2,
        UNEXPECTED_SYMBOL = 3,
        MISSING_FINISH_SIGN = 4
    };

    enum SemantizerError {
        SEMANTIC_ERROR = 5,
        REDIFINED_VAR = 6
    };

    const map<int, string> humanReadableErrors =
    {
        { LexerError::BAD_LITERA, "Unknown litera"},
        { LexerError::BAD_CONSTANT, "Bad constant"},
        { SyntaxerError::SYNTAX_ERROR, "Syntax error"},
        { SyntaxerError::UNEXPECTED_SYMBOL, "Unexpected symbol"},
        { SyntaxerError::MISSING_FINISH_SIGN, "Missing \".\" after \"end\""},
        { SemantizerError::SEMANTIC_ERROR, "Semantic error"},
        { SemantizerError::REDIFINED_VAR, "Variable is already defined"},
    };


    const map<string, Tag> keywords = {
        {"program",  Tag::PROGRAM},
        {"if",  Tag::IF},
        {"else",  Tag::ELSE},
        {"while",  Tag::WHILE},
        {"case",  Tag::CASE},
        {"repeat",  Tag::REPEAT},
        {"until",  Tag::UNTIL},
        {"do",  Tag::DO},
        {"break",  Tag::BREAK},
        {"function",  Tag::FUNCTION},
        {"procedure",  Tag::PROCEDURE},
        {"begin",  Tag::BEGIN},
        {"end",  Tag::END},
        {"integer",  Tag::INT},
        {"bool",  Tag::BOOL},
        {"char",  Tag::CHAR},
        {"float",  Tag::FLOAT},
        {"string",  Tag::STRING},
        {"or",  Tag::OR},
        {"and",  Tag::AND},
        {"then",  Tag::THEN},
        {"of",  Tag::OF},
        {"var",  Tag::VAR},
        {"type",  Tag::TYPE},
        {"for",  Tag::FOR},
        {"to",  Tag::TO},
        {"downto",  Tag::DOWNTO},
        {"record",  Tag::RECORD},
        {"const",  Tag::CONSTANT},
        {"forword",  Tag::FORWORD},
    };

    enum State {
        _START,
        _END,
        _INT,
        _FLOAT,
        _STRING,
        _IDENT
    };

    const char delimeters[] = { ',', '.', '(', ')', '[', ']', ';', ':', '=', '<', '>', '+', '-', '*', '/', '&' };

    class Token
    {
    public:
        comp::Tag tag;
        string value;
        int lineIndex;
        bool operator=(const Token& t);
        Token();
        Token(comp::Tag tag, string value, int lineIndex);
        Token(const Token& t);
    };

}
