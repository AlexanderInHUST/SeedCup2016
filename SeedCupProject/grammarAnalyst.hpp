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
    void analyse(vector<Token> * tokens);
    GrammarAnalyst(vector<Token> * tokens);
private:
    int handleAssignment(IteratorManager * manager);
    void handleInt(IteratorManager * manager);
    void handleIf(IteratorManager * manager);
    //void handleFor(IteratorManager * manager);
    //void handleDo(IteratorManager * manager);
    void handleWhile(IteratorManager * manager);
    void handleAnnotation(IteratorManager * manager);
    void handlePrintf(IteratorManager * manager);
    int handleExpression(IteratorManager * manager);
    void handleCurrentIt(IteratorManager * manager);
    vector<Token>::iterator getTheEndOfIf(IteratorManager * manager);
    vector<Token>::iterator getTheEndOfWhile(IteratorManager * manager);
    //vector<Token>::iterator getTheEndOfDo(IteratorManager * manager);
    void cleanNewVariable();
    vector<Token>::iterator it;
    Calculator calculator;
    MemoryStack memoryStack;
    stack<vector<string>> newVariable;
};
#endif /* grammarAnalyst_hpp */
