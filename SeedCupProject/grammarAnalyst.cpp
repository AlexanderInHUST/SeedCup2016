//
//  grammarAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "grammarAnalyst.hpp"

GrammarAnalyst::GrammarAnalyst(vector<Token> * tokens):calculator(&memoryStack){
    calculator = Calculator(&memoryStack);
}

void GrammarAnalyst::analyse(vector<Token> * tokens){
    IteratorManager manager(tokens);
    for(manager.jumpTo(tokens->begin()); manager.getIt() != tokens->end(); ){
        handleCurrentIt(&manager);
    }
    cleanNewVariable();
}

void GrammarAnalyst::handleCurrentIt(IteratorManager * manager){
    if(manager->getIt()->describe.compare("boundary") == 0){
        if(manager->getIt()->content.compare("//") == 0 || manager->getIt()->content.compare("/*") == 0)
            handleAnnotation(manager);
    }
    else if(manager->getIt()->describe.compare("keyword") == 0){
        if(manager->getIt()->content.compare("while") == 0){
            handleWhile(manager);
        }
        else if(manager->getIt()->content.compare("if") == 0){
            handleIf(manager);
        }
        else if(manager->getIt()->content.compare("for") == 0){
            //handleFor(manager);
        }
        else if(manager->getIt()->content.compare("int") == 0){
            handleInt(manager);
        }
        else if(manager->getIt()->content.compare("do") == 0){
            //handleDo(&manager);
        }
        else if(manager->getIt()->content.compare("break") == 0){
            for(int i = 0; i < newVariable.top().size(); i++){
                memoryStack.popVariable(newVariable.top()[i]);
            }
            newVariable.pop();
            return;
        }
    }
    else if(manager->getIt()->describe.compare("identf") == 0){
        if(manager->getIt()->content.compare("printf") == 0){
            handlePrintf(manager);
        }
        else{
            handleAssignment(manager);
        }
        
    }else if(manager->getIt()->describe.compare("note") == 0){
        manager->jump(1);
    }

}

// 赋值式的处理
int GrammarAnalyst::handleAssignment(IteratorManager * manager){
    bool isLast = true;
    vector<Token>::iterator temp = manager->getIt();
    do{
        manager->jump(1);
        if(manager->getIt()->content.compare("=") == 0)
            isLast = false;
    }while(manager->getIt()->content.compare(";") != 0);
    manager->jumpTo(temp);
    if(isLast){
        int value = handleExpression(manager);
        manager->move(1);
        return value;
    }
    else{
        string name = manager->getIt()->content;
        manager->move(2);
        int value = handleAssignment(manager);
        memoryStack.setVariable(name, value);
        return value;
    }
}

// At next token
void GrammarAnalyst::handleAnnotation(IteratorManager * manager){
    if(manager->getIt()->content.compare("//")){
        while((manager->getIt() + 1)->line == manager->getIt()->line){
            manager->jump(1);
        }
    }
    else{
        while(manager->getIt()->content != "*/"){
            manager->jump(1);
        }
    }
    manager->move(1);
}

// At next token
void GrammarAnalyst::handleInt(IteratorManager * manager){
    vector<string> variableInState;
    manager->move(1);
    while(manager->getIt()->describe.compare("identf") == 0){
        variableInState.insert(variableInState.end(), manager->getIt()->content);
        if((manager->getIt() + 1)->content == ";"){
            memoryStack.pushVariable(manager->getIt()->content, 0);
            manager->move(1);
            break;
        }
        else if((manager->getIt() + 1)->content == "="){
            string name = manager->getIt()->content;
            manager->move(2);
            memoryStack.pushVariable(name, handleExpression(manager));
        }
        else if((manager->getIt() + 1)->content == ","){
            memoryStack.pushVariable(manager->getIt()->content, 0);
            manager->move(2);
        }
    }
    newVariable.push(variableInState);
    manager->move(1);
}

// At next token
void GrammarAnalyst::handleIf(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfIfBorder = false;
    bool expression;
    vector<Token>::iterator startOfIf = manager->getIt();
    vector<Token>::iterator endOfIt;
    
    // To get the end iterator of if block
    endOfIt = getTheEndOfIf(manager);
    manager->jumpTo(startOfIf);
    
    manager->move(2);
    expression = (handleExpression(manager) == 0) ? false : true;
    manager->move(1);
    if(manager->getIt()->content == "{"){
        flagOfIfBorder = true;
        manager->move(1);
    }
    if(!flagOfIfBorder){
        if(expression){
            handleCurrentIt(manager);
        }
        else{
            while((manager->getIt() + 1)->line == manager->getIt()->line){
                manager->jump(1);
            }
            manager->jump(1);
            if(manager->getIt()->content.compare("else") == 0){
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    handleCurrentIt(manager);
                }
                else{
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    manager->jump(1);
                    analyse(&subTokens);
                }
            }
        }
    }else{
        if(!expression){
            while(!(manager->getIt()->content == "}" && stack == 0)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
            
            if(manager->getIt()->content.compare("else") == 0){
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    handleCurrentIt(manager);
                }
                else{
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    stack = 0;
                    manager->jump(1);
                    analyse(&subTokens);
                }
            }
        }
        else{
            while(!(manager->getIt()->content == "}" && stack == 0)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
            analyse(&subTokens);
        }
    }
    manager->jumpTo(endOfIt);
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfIf(IteratorManager * manager){
    int stack = 0;
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        while(!(manager->getIt()->content == "}" && stack == 1)){
            if(manager->getIt()->content == "{")
                stack++;
            else if(manager->getIt()->content == "}")
                stack--;
            manager->jump(1);
        }
        stack = 0;
        manager->jump(1);
    }
    else{
        while((manager->getIt() + 1)->line == manager->getIt()->line){
            manager->jump(1);
        }
        manager->jump(1);
    }
    while(manager->getIt()->content.compare("else") == 0){
        manager->jump(1);
        if(manager->getIt()->content.compare("if") == 0){
            while(manager->getIt()->content.compare(")") != 0)
                manager->jump(1);
            manager->jump(1);
        }
        if(manager->getIt()->content.compare("{") == 0){
            while(!(manager->getIt()->content == "}" && stack == 1)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
        }
        else{
            while((manager->getIt() + 1)->line == manager->getIt()->line){
                manager->jump(1);
            }
            manager->jump(1);
        }
    }
    return manager->getIt();
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
void GrammarAnalyst::handleWhile(IteratorManager * manager){
    vector<Token>  subTokens;
    int stack = 0;
    bool expression;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    vector<Token>::iterator startOfIt;
    manager->move(2);
    while(handleExpression(manager) == 1){
        manager->move(1);
        if(manager->getIt()->content == "{"){
            flagOfBorder = true;
            manager->move(1);
        }
        if(!flagOfBorder && !flagOfRead){
            while((manager->getIt() + 1)->line == manager->getIt()->line){
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
        }
        else if(!flagOfRead){
            while(!(manager->getIt()->content == "}" && stack == 0)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->move(1);
            }
        }
        manager->move(1);
        startOfIt = manager->getIt();
        analyse(&subTokens);
        manager->jumpTo(startOfIt);
    }
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfWhile(IteratorManager * manager){
    int stack = 0;
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        while(!(manager->getIt()->content == "}" && stack == 1)){
            if(manager->getIt()->content == "{")
                stack++;
            else if(manager->getIt()->content == "}")
                stack--;
            manager->jump(1);
        }
        manager->jump(1);
        
    }
    else{
        while((manager->getIt() + 1)->line == manager->getIt()->line){
            manager->jump(1);
        }
        manager->jump(1);
    }
    return manager->getIt();
}


// At next token
void GrammarAnalyst::handlePrintf(IteratorManager * manager){
    manager->move(5);
    while(manager->getIt()->content == ","){
        manager->move(1);
        handleExpression(manager);
    }
    manager->move(2);
}

// At next token

int GrammarAnalyst::handleExpression(IteratorManager * manager){
    vector<Token> expression;
    int value;
    while(manager->getIt()->describe != "boundary"){
        expression.insert(expression.end(), *manager->getIt());
        manager->move(1);
    }
    value =  calculator.doCalculator(expression);
    memoryStack.handleSideEffect(calculator.side_effect);
    calculator.cleanEffect();
    return value;
}

void GrammarAnalyst::cleanNewVariable(){
    for(int i = 0 ; i < newVariable.top().size(); i++){
        memoryStack.popVariable(newVariable.top()[i]);
    }
    newVariable.pop();
}
