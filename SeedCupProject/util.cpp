//
//  util.cpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/31.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#include "util.hpp"

vector<int> Util::result = vector<int>();

void Util::getResult(int line){ // 将结果输入到静态的vector中，如果发现两个相邻的输入是相同的，那么就只输一个
    if(result.empty()){
        result.insert(result.end(), line);
    }
    else{
        int lastResult = result[result.size() - 1];
        if(lastResult != line)
            result.insert(result.end(), line);
    }
}

void Util::printResult(){ // 根据要求改变输出方式即可
    for(int i = 0; i < result.size(); i++){
        cout << result[i] << " ";
    }
}
