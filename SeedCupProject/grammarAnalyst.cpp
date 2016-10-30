//
//  grammarAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "grammarAnalyst.hpp"

GrammarAnalyst::GrammarAnalyst(vector<Token> * tokens):manager(tokens),calculator(&memoryStack){
    manager = IteratorManager(tokens);
    calculator = Calculator(&memoryStack);
}

void GrammarAnalyst::analyse(vector<Token> * tokens){
    for(manager.jumpTo(tokens->begin()); manager.getIt() != tokens->end(); ){
        if(manager.getIt()->describe.compare("boundary") == 0){
            if(manager.getIt()->content.compare("//") == 0 || manager.getIt()->content.compare("/*") == 0)
                handleAnnotation();
        }
        else if(manager.getIt()->describe.compare("keyword") == 0){
            if(manager.getIt()->content.compare("while") == 0){
                handleWhile();
            }
            else if(manager.getIt()->content.compare("if") == 0){
                handleIf();
            }
            else if(manager.getIt()->content.compare("for") == 0){
                //handleFor();
            }
            else if(manager.getIt()->content.compare("int") == 0){
                handleInt();
            }
            else if(manager.getIt()->content.compare("do") == 0){
                //handleDo();
            }
            else if(manager.getIt()->content.compare("break") == 0){
                for(int i = 0; i < newVariable.top().size(); i++){
                    memoryStack.popVariable(newVariable.top()[i]);
                }
                newVariable.pop();
                return;
            }
        }
        else if(manager.getIt()->describe.compare("identf") == 0){
            if(manager.getIt()->content.compare("printf") == 0){
                handlePrintf();
            }
                
        }else{
            manager.move(1);
        }
    }
}

// At next token
void GrammarAnalyst::handleAnnotation(){
    if(manager.getIt()->content.compare("//")){
        while((manager.getIt() + 1)->line == manager.getIt()->line){
            manager.jump(1);
        }
    }
    else{
        while(manager.getIt()->content != "*/"){
            manager.jump(1);
        }
    }
    manager.move(1);
}

// At next token
void GrammarAnalyst::handleInt(){
    vector<string> variableInState;
    manager.move(1);
    while(manager.getIt()->describe.compare("identf") == 0){
        variableInState.insert(variableInState.end(), manager.getIt()->content);
        if((manager.getIt() + 1)->content == ";"){
            memoryStack.pushVariable(manager.getIt()->content, 0);
            manager.move(1);
            break;
        }
        else if((manager.getIt() + 1)->content == "="){
            string name = manager.getIt()->content;
            manager.move(2);
            memoryStack.pushVariable(name, handleExpression());
        }
        else if((manager.getIt() + 1)->content == ","){
            memoryStack.pushVariable(manager.getIt()->content, 0);
            manager.move(2);
        }
    }
    newVariable.push(variableInState);
    manager.move(1);
}

// Fix me in case of "else if"
// At next token
void GrammarAnalyst::handleIf(){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool expression = true;
    vector<Token>::iterator oldIt;
    manager.move(2);
    if(handleExpression() == 0){
        expression = false;
        while(manager.getIt()->content != "else")
            manager.jump(1);
        manager.jump(1);
    }
    if(manager.getIt()->content == "{"){
        flagOfBorder = true;
        manager.move(1);
    }
    if(!flagOfBorder){
        while((manager.getIt() + 1)->line == manager.getIt()->line){
            subTokens.insert(subTokens.end(), *manager.getIt());
            manager.jump(1);
        }
    }
    else{
        while(!(manager.getIt()->content == "}" && stack == 0)){
            if(manager.getIt()->content == "{")
                stack++;
            else if(manager.getIt()->content == "}")
                stack--;
            subTokens.insert(subTokens.end(), *manager.getIt());
            manager.jump(1);
        }
    }
    manager.move(1);
    oldIt = manager.getIt();
    analyse(&subTokens);
    manager.jumpTo(oldIt);
    if(expression){
        if(manager.getIt()->content.compare("else")){
            if(manager.getIt()->content == "{"){
                flagOfBorder = true;
                manager.jump(1);
            }
            if(!flagOfBorder){
                while((manager.getIt() + 1)->line == manager.getIt()->line)
                    manager.jump(1);
            }
            else{
                while(!(manager.getIt()->content == "}" && stack == 0)){
                    if(manager.getIt()->content == "{")
                        stack++;
                    else if(manager.getIt()->content == "}")
                        stack--;
                    manager.jump(1);
                }
            }
            manager.jump(1);
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
    manager.move(2);
    while(handleExpression() == 1){
        if(manager.getIt()->content == "{"){
            flagOfBorder = true;
            manager.move(1);
        }
        if(!flagOfBorder && !flagOfRead){
            while((manager.getIt() + 1)->line == manager.getIt()->line){
                subTokens.insert(subTokens.end(), *manager.getIt());
                manager.jump(1);
            }
        }
        else if(!flagOfRead){
            while(!(manager.getIt()->content == "}" && stack == 0)){
                if(manager.getIt()->content == "{")
                    stack++;
                else if(manager.getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager.getIt());
                manager.move(1);
            }
        }
        manager.move(1);
        oldIt = manager.getIt();
        analyse(&subTokens);
        manager.jumpTo(oldIt);
    }
}

// At next token
void GrammarAnalyst::handlePrintf(){
    manager.move(2);
    while(manager.getIt()->describe == "identf"){
        manager.move(1);
    }
    while(manager.getIt()->content == ","){
        manager.move(1);
        handleExpression();
    }
    manager.move(2);
}

// At next token

int GrammarAnalyst::handleExpression(){
    vector<Token> expression;
    int value;
    while(manager.getIt()->describe != "boundary"){
        expression.insert(expression.end(), *manager.getIt());
        manager.move(1);
    }
    value =  calculator.doCalculator(expression);
    memoryStack.handleSideEffect(calculator.side_effect);
    calculator.cleanEffect();
    return value;
}
