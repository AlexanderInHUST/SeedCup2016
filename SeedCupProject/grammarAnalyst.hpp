//
//  grammarAnalyst.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/29.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef grammarAnalyst_hpp
#define grammarAnalyst_hpp

#include "token.hpp"
#include "memoryStack.hpp"
#include "calculator.hpp"
#include "iteratorManager.hpp"

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

#define NORMAL_END 1
#define BREAK_END -1

using namespace std;

//语法分析器
class GrammarAnalyst{
public:
    int analyse(vector<Token> * tokens); //分析传入的Token流的语法流程
    GrammarAnalyst(); //构造函数
private:
    //下面分析语句返回值为NORMAL_END时，说明是正常结束；为BREAK_END时，说明是因为break语句导致的结束。
    int handleAssignment(IteratorManager * manager); //分析并处理赋值语句
    int handleInt(IteratorManager * manager); //分析处理int初始化语句
    int handleIf(IteratorManager * manager); //分析处理if语句块
    int handleFor(IteratorManager * manager); //分析处理for语句块
    int handleDo(IteratorManager * manager); //分析处理do-while语句块
    int handleWhile(IteratorManager * manager); //分析处理while语句块
    int handlePrintf(IteratorManager * manager); //分析处理printf语句
    int handleExpression(IteratorManager * manager); //分析处理表达式语句
    int handleSelfChange(IteratorManager * manager); //分析处理自增自减语句
    int handleCurrentIt(IteratorManager * manager); //分析当前Token所示的语句块，调用上面的某一个方法
    
    vector<Token>::iterator getTheEndOfIf(IteratorManager * manager); //得到if语句块结束的Iterator
    vector<Token>::iterator getTheEndOfWhileOrFor(IteratorManager * manager); //得到for\while语句块结束的Iterator
    vector<Token>::iterator getTheEndOfDo(IteratorManager * manager); //得到do-while语句块结束的Iterator
    vector<Token>::iterator getTheEndOfBlock(IteratorManager * manager); //分析当前Token所示的语句块，调用上面的某一个方法
    vector<Token>::iterator getTheEndOfBracket(IteratorManager * manager); //得到{}的终点之后的token
    
    int handleExpressionInFor(IteratorManager * manager); //单独处理for中可能出现的逗号表达式
    void cleanNewVariable(); //清理本次作用域声明的变量的值
    Calculator calculator; //计算所有表达式的Calculator
    MemoryStack memoryStack; //存储所有变量值的MemoryStack
    stack<vector<string>> newVariable; //作用域中的变量名栈，用来表示本次作用域中声明的变量
};
#endif /* grammarAnalyst_hpp */
