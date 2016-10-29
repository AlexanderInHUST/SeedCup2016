//
//  wordAnalyst.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef wordAnalyst_hpp
#define wordAnalyst_hpp

#include "token.hpp"
#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

class WordAnalyst{
public:
    vector<Token> tokens;
    void analyst();
    WordAnalyst(string code);
private:
    string code;
    int currentLine;
    void insertToken(Token token);
};

#endif /* wordAnalyst_hpp */
