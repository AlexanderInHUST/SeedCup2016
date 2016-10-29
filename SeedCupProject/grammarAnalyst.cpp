//
//  grammarAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "grammarAnalyst.hpp"

GrammarAnalyst::GrammarAnalyst(vector<Token> tokens):manager(&tokens),calculator(&memoryStack){
    //Do nothing...
}

void GrammarAnalyst::analyse(vector<Token> tokens){
    for(it = tokens.begin(); it != tokens.end(); it++){
        if(it->type.compare("Border") == 0){
            if(it->self.compare("//") == 0 || it->self.compare("/*") == 0)
                handleAnnotation();
        }
        else if(it->type.compare("ReservedWord")){
            if(it->self.compare("while") == 0){
                handleWhile();
            }
            else if(it->self.compare("if") == 0){
                handleIf();
            }
            else if(it->self.compare("for") == 0){
                //handleFor();
            }
            else if(it->self.compare("printf") == 0){
                handlePrintf();
            }
            else if(it->self.compare("int") == 0){
                handleInt();
            }
            else if(it->self.compare("do") == 0){
                //handleDo();
            }
            else if(it->self.compare("break") == 0){
                for(int i = 0; i < newVariable.top().size(); i++){
                    memoryStack.popVariable(newVariable.top()[i]);
                }
                newVariable.pop();
                return;
            }
        }
    }
}

// At next token
void GrammarAnalyst::handleAnnotation(){
    if(it->self.compare("//")){
        while((it + 1)->line == it->line){
            it++;
        }
    }
    else{
        while(it->self != "*/"){
            it++;
        }
    }
    it++;
}

// At next token
void GrammarAnalyst::handleInt(){
    vector<string> variableInState;
    it++;
    while(it->type.compare("Variable") == 0){
        variableInState.insert(variableInState.end(), it->self);
        if((it + 1)->self == ";"){
            it++;
            break;
        }
        else if((it + 1)->self == "="){
            string name = it->self;
            it = it + 2;
            memoryStack.pushVariable(name, handleExpression());
        }
        else if((it + 1)->self == ","){
            it++;
            memoryStack.pushVariable(it->self, 0);
        }
    }
    newVariable.push(variableInState);
}

// At next token
void GrammarAnalyst::handleIf(){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool expression = true;
    vector<Token>::iterator oldIt;
    it = it + 2;
    if(handleExpression() == 0){
        expression = false;
        while(it->self != "else")
            it++;
        it++;
    }
    if(it->self == "{"){
        flagOfBorder = true;
        it++;
    }
    if(!flagOfBorder){
        while((it + 1)->line == it->line){
            subTokens.insert(subTokens.end(), *it);
            it++;
        }
    }
    else{
        while(!(it->self == "}" && stack == 0)){
            if(it->self == "{")
                stack++;
            else if(it->self == "}")
                stack--;
            subTokens.insert(subTokens.end(), *it);
            it++;
        }
    }
    oldIt = ++it;
    analyse(subTokens);
    it = oldIt;
    if(expression){
        if(it->self.compare("else")){
            if(it->self == "{"){
                flagOfBorder = true;
                it++;
            }
            if(!flagOfBorder){
                while((it + 1)->line == it->line)
                    it++;
            }
            else{
                while(!(it->self == "}" && stack == 0)){
                    if(it->self == "{")
                        stack++;
                    else if(it->self == "}")
                        stack--;
                    it++;
                }
            }
            it++;
        }
    }
    return;
}

// At next token
/*
void GrammarAnalyst::handleDo(){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    vector<Token>::iterator oldIt;
    it++;
    do{
        if()
    }
}
 */

// At next token
void GrammarAnalyst::handleWhile(){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    vector<Token>::iterator oldIt;
    it = it + 2;
    while(handleExpression() == 1){
        if(it->self == "{"){
            flagOfBorder = true;
            it++;
        }
        if(!flagOfBorder && !flagOfRead){
            while((it + 1)->line == it->line){
                subTokens.insert(subTokens.end(), *it);
                it++;
            }
        }
        else if(!flagOfRead){
            while(!(it->self == "}" && stack == 0)){
                if(it->self == "{")
                    stack++;
                else if(it->self == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *it);
                it++;
            }
        }
        oldIt = ++it;
        analyse(subTokens);
        it = oldIt;
    }
}

// At next token
void GrammarAnalyst::handlePrintf(){
    while(it->self != "\""){
        it++;
    }
    it++;
    while(!(it->self == "\"" && (it-1)->self != "\\")){
        it++;
    }
    it++;
    while(it->self == ","){
        handleExpression();
    }
    it = it + 2;
}

// At next token

int GrammarAnalyst::handleExpression(){
    vector<Token> expression;
    int value;
    while(it->type != "Border"){
        expression.insert(expression.end(), *it);
        it++;
    }
    it++;
    value =  calculator.doCalculator(expression);
    memoryStack.handleSideEffect(calculator.side_effect);
    calculator.cleanEffect();
    return value;
}
