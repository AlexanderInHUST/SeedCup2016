//
//  iteratorManager.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "iteratorManager.hpp"

IteratorManager::IteratorManager(vector<Token> *tokens){
    this->tokens = *tokens;
    this->origin = tokens;
    it = this->tokens.begin();
    tokensBegin = this->it;
    originBegin = this->origin->begin();
    lastLine = -1;
}

vector<Token>::iterator IteratorManager::getIt(){
    return transferOut(it);
}

void IteratorManager::move(int step){
    for (int i = 0; i < step; i++){
        if(it > tokens.end())
            it = tokens.end();
        else
            it++;
        printLine();
    }
}

void IteratorManager::moveTo(vector<Token>::iterator aim){
    while (it != transferIn(aim)){
        if(it > tokens.end())
            it = tokens.end();
        else
            it++;
        printLine();
    }
}

void IteratorManager::jump(int step){
    it = it + step;
    if(it > tokens.end())
        it = tokens.end();
}

void IteratorManager::jumpTo(vector<Token>::iterator aim){
    it = transferIn(aim);
}

//考虑输出结果后，如果当前line与前一个考虑输出的结果的line不同，才进行输出
void IteratorManager::printLine(){
    if (lastLine != it->line){
        Util::getResult(it->line);
    }
    lastLine = it->line;
}

//通过计算aim与相应的vector的begin的差值，就可以获得另一个vector所对应的位置
vector<Token>::iterator IteratorManager::transferIn(vector<Token>::iterator aim){
    long length = aim - originBegin;
    return tokens.begin() + length;
}

vector<Token>::iterator IteratorManager::transferOut(vector<Token>::iterator aim){
    long length = aim - tokensBegin;
    return origin->begin() + length;
}

vector<Token>::iterator IteratorManager::getEnd(){
    return origin->end();
}