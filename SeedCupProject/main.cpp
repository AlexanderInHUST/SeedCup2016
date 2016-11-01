//
//  main.cpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/24.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.

#include "calculator.hpp"
#include "memoryStack.hpp"
#include "wordAnalyst.hpp"
#include "grammarAnalyst.hpp"

#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace::std;

int main(int argc, const char * argv[]) {
    WordAnalyst wordAnalyst;
    GrammarAnalyst grammarAnalyst;
    vector<Token> tokens;
    tokens = wordAnalyst.getTokens(); // 词法分析
    grammarAnalyst.analyse(&tokens); // 语法分析
    Util::printResult(); //输出结果
    return 0;
}
