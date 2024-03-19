#pragma once
#include "common.h"

struct LexerToken{
    std::string token;
    size_t length;
    unsigned short int lineNumber;
    unsigned short int type;
    struct LexerToken *next;
};

//Linked list to hold lexer tokens
class LexerList {
public:
    LexerList(std::string keywordsFile, std::string symbolsFile);

    void addToken(std::string token, int lineNumber, int type){
        size_t length = token.size();
        createToken(token, length, lineNumber, type);
    }

    inline bool checkKeyword(std::string token){
        if(std::find(keywords.begin(),  keywords.end(), token) != keywords.end()) return true;
        return false;
    }

    inline bool checkSymbol(std::string token){
        if(std::find(symbols.begin(),  symbols.end(), token) != symbols.end()) return true;
        return false;
    }

    void printList(){
        LexerToken *currentToken = head;
        while(currentToken != tail){
            std::cout << ((currentToken->token == "\n") ? "\\n" : currentToken->token) << " " << currentToken->length << " " << currentToken->lineNumber << " " << currentToken->type << std:: endl;
            currentToken = currentToken->next;
        }
        std::cout << ((currentToken->token == "\n") ? "\\n" : currentToken->token) << " " << currentToken->length << " " << currentToken->lineNumber << " " << currentToken->type << std:: endl;
    }

private:
    LexerToken *head;
    LexerToken *tail;
    std::vector<std::string> keywords;
    std::vector<std::string> symbols;

    void createToken(std::string token, size_t length, int lineNumber, int type){
        LexerToken *newToken = new LexerToken;
        newToken->token = token;
        newToken->length = length;
        newToken->lineNumber = lineNumber;
        newToken->type = type;
        if(head == nullptr){
            head = newToken;
            tail = newToken;
            return;
        }
        tail->next = newToken;
        tail = newToken;
    }
};

LexerList* lex(std::string file);
