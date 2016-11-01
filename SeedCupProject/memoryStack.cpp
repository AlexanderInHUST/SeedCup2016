//
//  memoryStack.cpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/27.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#include "memoryStack.hpp"

using namespace std;

//如果已经出现了这个变量了，就直接在它所拥有的栈中push，否则先insert，再push
void MemoryStack::pushVariable(string name, int value){
    map<string, stack<int>>::iterator it = memory.find(name);
    if(it != memory.end()){
        stack<int> *variable = &it->second;
        variable->push(value);
    }else{
        stack<int> variable;
        variable.push(value);
        memory.insert(pair<string, stack<int>>(name, variable));
    }
}

void MemoryStack::popVariable(string name){
    map<string, stack<int>>::iterator it = memory.find(name);
    stack<int> *variable = &it->second;
    variable->pop();
    if(variable->empty()){
        memory.erase(it);
    }
}

void MemoryStack::setVariable(string name, int value){
    popVariable(name);
    pushVariable(name, value);
}

int MemoryStack::getVariable(string name){
    map<string, stack<int>>::iterator it = memory.find(name);
    if(it != memory.end()){
        return it->second.top();
    }
    return 0;
}

//遍历传入的副作用KV表，依次改变所对应的值即可
void MemoryStack::handleSideEffect(map<string, int> sideEffect){
    for(map<string, int>::iterator it = sideEffect.begin(); it != sideEffect.end(); it++){
        setVariable(it->first, getVariable(it->first) + it->second);
    }
}


