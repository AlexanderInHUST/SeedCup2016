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

using namespace std;

//string allTypes[8] = {"ReservedWord", "Variable", "DataType", "Symbols",
//"Border", "End", "NextLine", "Annotation"};


class GrammarAnalyst{
public:
    void analyse(vector<Token> tokens);
    GrammarAnalyst(vector<Token> tokens);
private:
    void handleInt();
    void handleIf();
    //void handleFor();
    //void handleDo();
    void handleWhile();
    void handleAnnotation();
    void handlePrintf();
    int handleExpression();
    vector<Token>::iterator it;
    Calculator calculator;
    MemoryStack memoryStack;
    IteratorManager manager;
    stack<vector<string>> newVariable;
};
#endif /* grammarAnalyst_hpp */
