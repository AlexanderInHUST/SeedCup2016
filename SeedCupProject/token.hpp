//
//  token.hpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#ifndef token_hpp
#define token_hpp

#include <stdio.h>

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

/*  type: variable
 *  name: a
 *  value: 1
 *  line: 1 
 */

class Token{
public:
    string type;
    string self;
    int line;
    Token(string self, int line, int type);
private:
    string allTypes[6] = {"ReservedWord", "Variable", "Constant", "Symbols", "Border", "End"};
    string allReservedWord[8] = {"while", "if", "break", "printf", "int", "do", "else", "for"};
    string allSymbols[12] = {"+", "-", "*", "/", "++", "--", ">", "<", ">=", "<=", "!=", "=="};
    string allBorder[10] = {"{", "}", "\"", "\'", "(", ")", ",", "/*", "*/" ,"//"};
};

#endif /* token_hpp */
