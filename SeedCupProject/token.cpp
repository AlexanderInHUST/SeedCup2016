//
//  token.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/28.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "token.hpp"

Token::Token(string self, int line, int type){
    this->self = self;
    this->line = line;
    this->type = allTypes[type];
}
