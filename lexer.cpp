#include "common.h"

LexerList::LexerList(std::string keywordsFile, std::string symbolsFile){
    head = nullptr;
    tail = nullptr;

    std::ifstream keywords (keywordsFile);
    std::ifstream symbols (symbolsFile);

    if(!keywords.is_open() || !symbols.is_open()) throw std::invalid_argument("Invalid/missing keywords and/or symbols file");

    std::string buffer;
    while(keywords.peek() != EOF){
        std::getline(keywords, buffer);
        this->keywords.push_back(buffer);
    }

    while(symbols.peek() != EOF){
        std::getline(symbols, buffer);
        this->symbols.push_back(buffer);
    }
}

LexerList* lex(std::string file){
    //Open file
    std::ifstream source (file);
    std::string buffer;

    LexerList *lexList = new LexerList(DEFAULT_KEYWORDS_FILE, DEFAULT_SYMBOLS_FILE);

    if(!source.is_open()) { std::cerr << "Couldn't open file " << file << std::endl; throw std::invalid_argument("Unable to open file");}

    //Remove comments from source code
    while(source.peek() != EOF){

        //Get a character
        char current = source.get();

        //If the character + next character result in a single line comment, skip till the next newline
        if(current == DEFAULT_COMMENT_VALUE[0] && source.peek() == DEFAULT_COMMENT_VALUE[1]){
            while(source.get() != '\n');
            buffer += '\n'; //Emit the newline that was skipped
            continue;
        }

        //If char + next char is a multiline comment, skip until the multiline comment ends
        if(current == MULTILINE_COMMENT_START[0] && source.peek() == MULTILINE_COMMENT_START[1]){
            while(!(source.get() == MULTILINE_COMMENT_END[0] && source.peek() == MULTILINE_COMMENT_END[1]));
            source.ignore(); //Ignore the second character for multiline end
            continue;
        }

        //Emit the current value if it's not contained within a comment
        buffer += current;
    }

    int lineNumber = 1;

    std::string lexeme;
    int type = 0;

    //For each character in the buffer
    for(size_t i = 0; i < buffer.size(); i++){
        char c = buffer[i];

        //If newline increment line number
        if(c == '\n') lineNumber++;

        //Determine type if not already determined
        if(type == 0){

            //Ignore whitespace
            if(c == '\n' || c == ' ' || c == '\t') continue;

            //Integer/float
            else if(c > 47 && c < 58) type = 1;

            //Keyword/identifier
            else if((c > 64 && c < 91) || (c > 96 && c < 123) || c == 95) type = 3;

            //String
            else if(c == '"') type = 5;

            //Symbol
            else type = 6;
        }

        switch(type){

            //If lexeme is integer
            case 1:

                //If still a number
                if(c > 47 && c < 58) lexeme += c;

                //If there's a decimal point, int becomes floating point
                else if(c == '.'){lexeme += c; type = 2;}

                else{
                    //Emit lexeme
                    lexList->addToken(lexeme, lineNumber, DEFAULT_INT_TYPE);
                    if(c != ' ') i--; //Only skip character if it's whitespace
                    type = 0;
                    lexeme = "";
                }
                break;

            //If lexeme is floating point
            case 2:

                //If still a number
                if(c > 47 && c < 58) lexeme += c;

                //A number with two decimal points is invalid
                else if(c == '.'){std::cerr << "Invalid integer at line " << lineNumber << std::endl; throw std::invalid_argument("Invalid integer");}

                else{
                    lexList->addToken(lexeme, lineNumber, DEFAULT_FLOAT_TYPE); //Emit lexeme
                    i--;
                    type = 0;
                    lexeme = "";
                }
                break;

            case 3:

                //If character is an uppercase or lowercase letter, an underscore or a digit
                if((c > 64 && c < 91) || (c > 96 && c < 123) || c == 95 || (c > 47 && c < 58)) lexeme += c;
                else{
                    //Identify whether lexeme is keyword or identifier and emit corresponding lexeme
                    lexList->addToken(lexeme, lineNumber, (lexList->checkKeyword(lexeme) ? DEFAULT_KEYWORD_TYPE : DEFAULT_IDENTIFIER_TYPE));
                    type = 0;
                    lexeme = "";
                    i--;
                }
                break;

            case 5:
                lexeme += c;
                if(lexeme != "\"" && c == '"'){
                    lexList->addToken(lexeme, lineNumber, DEFAULT_STRING_TYPE);
                    type = 0;
                    lexeme = "";
                }
                break;

            case 6:
                lexeme += c;
                if(lexList->checkSymbol(lexeme)){
                    lexList->addToken(lexeme, lineNumber, DEFAULT_SYMBOL_TYPE);
                    type = 0;
                    lexeme = "";
                }
                break;
        }
    }

    //This covers edgecase where last token is a keyword or identifier and makes sure it's added as a lexeme
    if(type == 3){
        lexList->addToken(lexeme, lineNumber, (lexList->checkKeyword(lexeme) ? DEFAULT_KEYWORD_TYPE : DEFAULT_IDENTIFIER_TYPE));
        lexeme = "";
    }

    if(lexeme != "") { std::cerr << "Lexeme not fully parsed: " << lexeme; throw std::invalid_argument("Lexeme not fully parsed"); }

    source.close();

    return lexList;
}
