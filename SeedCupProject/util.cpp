//
//  util.cpp
//  SeedCupProject
//
//  Created by 唐艺峰 on 16/10/31.
//  Copyright © 2016年 唐艺峰. All rights reserved.
//

#include "util.hpp"

// vector<int> resultVector;
// string resultString;

vector<int> Util::result = vector<int>();

void Util::getResult(int line){
    if(result.empty()){
        result.insert(result.end(), line);
    }
    else{
        int lastResult = result[result.size() - 1];
        if(lastResult != line)
            result.insert(result.end(), line);
    }
}

void Util::printResult(){
    for(int i = 0; i < result.size(); i++){
        cout << result[i] << " ";
    }
}
