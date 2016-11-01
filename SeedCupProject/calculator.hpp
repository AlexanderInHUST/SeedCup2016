//
//  calculator.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/27.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef calculator_hpp
#define calculator_hpp

#include "token.hpp"
#include "memoryStack.hpp"

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

//用于在Calculator内部计算时，分辨变量、常量以及自增的标志的储存类
class Num{
public:
    string name;
    int num;
    Num(string name, int num);
    Num(int num);
    Num();
};

//
class Calculator{
public:
    Calculator(MemoryStack * memory); //构造函数，传入公用来存储各个变量的MemoryStack指针
    map<string, int> side_effect; //一个用于记录在表达式计算中因为自增自减符号导致的副作用的KV表，string：变量名，int：副作用改变量
    int doCalculator(vector<Token> tokens); //对传入的tokens进行计算
    void cleanEffect(); //清除副作用KV表中的内容，防止对以后的表达式计算产生影响
    
private:
    vector<string> allSymbols; //储存所有的会在表达式中出现的符号
    map<string, int> priority; //储存所有的会在表达式中出现的符号的优先级
    void initialPriority(); //初始化优先级
    Num calculate(stack<Num> & result, string symbol); //计算当前状态下的值
    void prehandlerTokens(vector<Token> & tokens); //预处理Token流，处理其中的自增自减、在变量前面加负号的情况
    MemoryStack * memoryStack; //指向公用的MemoryStack
};

#endif /* calculator_hpp */
