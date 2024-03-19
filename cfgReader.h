#pragma once
#include "common.h"

class cfgReader{

public:
    cfgReader(std::string cfgFile);
    cfgReader();

private:
    std::string cfgFile;

    std::map<std::string, int> tokenNumbers;
    std::map<bool, int> isTerminal;

    void populateTokenNumbers(){
        std::ifstream source (this->cfgFile);
        std::string buffer;

        if(!source.is_open()) { std::cerr << "Unable to find grammar file: " << cfgFile << std::endl; throw std::invalid_argument("Unable to find grammar");}

        while(source.peek() != EOF){
            std::string line;
            std::getline(source, line);

            //Skip comments
            if(line.at(0) == ';') continue;

            std::string word = "";
            bool identifying = false;
            bool escaped = false;

            char c;

            for(size_t i = 0; i < line.size(); i++){
                c = line[i];

                if((c == '<' || c == '"') && identifying == false){
                    identifying = true;
                    continue;
                }

                if(identifying){

                    if(escaped){
                        word += c;
                        escaped = !escaped;
                        continue;
                    }
                    if(c == '\\'){
                        escaped = !escaped;
                        continue;
                    }
                    if(c == '>' || c == '"'){
                        identifying = false;
                        if(tokenNumbers.find(word) == tokenNumbers.end()) tokenNumbers.insert(std::pair<std::string, int>(word, tokenNumbers.size()));
                        else tokenNumbers.find(word); //TODO: REPLACE NUMBER WITH CORRESPONDING INT

                        std::cout << tokenNumbers.find(word)->first << " " << tokenNumbers.find(word)->second << std::endl;
                        word = "";
                        continue;
                    }
                    word += c;
                }
            }
        }
    };

    void parseCfg(){

    }
};
