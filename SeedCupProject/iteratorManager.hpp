//
//  iteratorManager.hpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/29.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#ifndef iteratorManager_hpp
#define iteratorManager_hpp

#include "token.hpp"
#include "util.hpp"

#include <stdio.h>
#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <strstream>

using namespace std;

//管理了主程序中遍历Token流的vector的Iterator，并且输出结果
//由于没办法直接对vector的Iterator进行操作，于是这里在manager内部存储了另一个一样的vector，操作时操作内部的vector，在转换成外部的vector的Iterator
class IteratorManager{
public:
    IteratorManager(vector<Token> *tokens); //构造函数，传入需要管理的Token流
    vector<Token>::iterator getIt(); //得到当前的Token的Iterator
    void move(int step); //Iterator向后移动step步，并且考虑输出结果
    void moveTo(vector<Token>::iterator aim); //Iterator向后一步一步移动到aim位置，并且考虑输出结果
    void jump(int step); //Iterator在不输出结果的情况下向后跳step步
    void jumpTo(vector<Token>::iterator aim); //Iterator在不输出结果的情况下跳到任一aim位置
    vector<Token>::iterator getEnd(); //得到管理的Token所在的vector的vector::end
private:
    vector<Token>::iterator it; //主Iterator
    vector<Token> tokens; //内部的vector
    vector<Token> *origin; //外部的vector的指针
    vector<Token>::iterator tokensBegin; //内部vector的vector::begin
    vector<Token>::iterator originBegin; //外部vector的vector::begin
    int lastLine; //之前考虑输出结果的line
    void printLine(); //输出结果
    vector<Token>::iterator transferIn(vector<Token>::iterator aim); //将外部Iterator转换到内部
    vector<Token>::iterator transferOut(vector<Token>::iterator aim); //将内部Iterator转换到外部
};

#endif /* iteratorManager_hpp */
