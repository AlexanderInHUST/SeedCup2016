//
//  wordAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "wordAnalyst.hpp"

//string allTypes[6] = {"ReservedWord", "Variable", "DataType", "Symbols", "Border", "End"};
//                          0              1            2           3         4       5
//string allReservedWord[8] = {"while", "if", "break", "printf", "int", "do", "else", "for"};
//string allSymbols[12] = {"+", "-", "*", "/", "++", "--", ">", "<", ">=", "<=", "!=", "=="};
//string allBorder[10] = {"{", "}", "\"", "\'", "(", ")", ",", "/*", "*/" ,"//"};
//string allEnd[2] = {";", "\n"};
//Token(string self, int line, int type);

using namespace std;

WordAnalyst::WordAnalyst(string code){
    this->code = code;
    currentLine = 1;
}

void WordAnalyst::analyst(){
    for(int i = 0; i < code.length(); i++){
        
        // With the beginning of '\n'
        if(code[i] == '\n'){
            insertToken(Token("\n", currentLine, 6));
            i++;
            currentLine++;
        }
        
        // With the beginning of '+'
        else if(code[i] == '+'){
            if(code[i + 1] == '+'){
                insertToken(Token("++", currentLine, 3));
                i++;
            }
            else
                insertToken(Token("+", currentLine, 3));
        }
        
        // With the beginning of '-'
        else if(code[i] == '-'){
            if(code[i + 1] == '-'){
                insertToken(Token("--", currentLine, 3));
                i++;
            }
            else
                insertToken(Token("-", currentLine, 3));
        }
        
        // With the beginning of '*'
        else if(code[i] == '*'){
            if(code[i + 1] == '/'){
                insertToken(Token("*/", currentLine, 4));
                i++;
            }
            else
                insertToken(Token("*", currentLine, 3));
        }
        
        // With the beginning of '/'
        else if(code[i] == '/'){
            if(code[i + 1] == '*'){
                insertToken(Token("/*", currentLine, 4));
                i++;
            }
            else if(code[i + 1] == '/'){
                insertToken(Token("//", currentLine, 4));
                i++;
                // To make the whole line annotated without '\n'
                while(code[i + 1] != '\n'){
                    insertToken(Token(string(1, code[i]), currentLine, 7));
                    i++;
                }
            }
            else
                insertToken(Token("/", currentLine, 3));
        }
        
        // With the beginning of '>'
        else if(code[i] == '>'){
            if(code[i + 1] == '='){
                insertToken(Token(">=", currentLine, 3));
                i++;
            }else
                insertToken(Token(">", currentLine, 3));
        }
        
        // With the beginning of '<'
        else if(code[i] == '<'){
            if(code[i + 1] == '<'){
                insertToken(Token("<=", currentLine, 3));
                i++;
            }else
                insertToken(Token("<", currentLine, 3));
        }
        
        // With the beginning of '!'
        else if(code[i] == '!'){
            if(code[i + 1] == '='){
                insertToken(Token("!=", currentLine, 3));
                i++;
            }
        }

    }
}

void WordAnalyst::insertToken(Token token){
    tokens.insert(tokens.end(), token);
}