//
//  util.hpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/31.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#ifndef util_hpp
#define util_hpp

#include "token.hpp"

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>


class Util{
public:
    void static getResult(int line); // 获取结果到result中
    void static printResult(); // 以要求的方式输出结果
private:
    static vector<int> result;
};

#endif /* util_hpp */
