//
//  memoryStack.hpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/27.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#ifndef memoryStack_hpp
#define memoryStack_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

//模拟变量树
class MemoryStack{
public:
    map<string, stack<int>> memory; //存储所有变量的值的一张KV表
    void popVariable(string name); //将变量名为name的变量所拥有的内存栈pop一次
    void pushVariable(string name, int value); //将value值push进变量名为name的变量所拥有的内存栈
    void setVariable(string name, int value); //将变量名为name的变量所拥有的内存栈top的值修改为value的值
    int getVariable(string name); //得到变量名为name的变量所拥有的内存栈的top
    void handleSideEffect(map<string, int> sideEffect); //将表达式计算因为自增自减得到的副作用处理到KV之中
private:
};

#endif /* memoryStack_hpp */
