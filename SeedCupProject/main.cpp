//
//  main.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/24.
//  Copyright © 2016年 唐艺峰. All rights reserved.

#include "calculator.hpp"
#include "memoryStack.hpp"
#include "fileHandler.hpp"
#include "wordAnalyst.hpp"

#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

//  string allTypes[8] = {"ReservedWord", "Variable", "Constant", "Symbols", "Border", "End"};

using namespace::std;

int main(int argc, const char * argv[]) {
    MemoryStack memoryStack;
    Calculator calculator(&memoryStack);
    vector<Token> tokens;
    memoryStack.pushVariable("test", 10);
    memoryStack.pushVariable("test_two", 20);
    tokens.insert(tokens.end(), Token("test", 0, 1));
    tokens.insert(tokens.end(), Token("++", 0, 3));
    tokens.insert(tokens.end(), Token("<", 0, 3));
    tokens.insert(tokens.end(), Token("test_two", 0, 1));
    cout << calculator.doCalculator(tokens) << endl;
    memoryStack.handleSideEffect(calculator.side_effect);
    calculator.cleanEffect();
    cout << memoryStack.getVariable("test") << " " << memoryStack.getVariable("test_two") << endl;
    return 0;
}
