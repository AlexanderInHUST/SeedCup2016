//
//  main.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/24.
//  Copyright © 2016年 唐艺峰. All rights reserved.

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
    MemoryStack memoryStack;
    Calculator calculator(&memoryStack);
    WordAnalyst wordAnalyst;
    vector<Token> tokens;
    tokens = wordAnalyst.getTokens();
    GrammarAnalyst grammarAnalyst;
    grammarAnalyst.analyse(&tokens);
    Util::printResult();
    return 0;
}
