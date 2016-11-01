//
//  calculator.cpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/27.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#include "calculator.hpp"

using namespace::std;

Calculator::Calculator(MemoryStack * memoryStack){
    initialPriority();
    this->memoryStack = memoryStack;
}

//使用调度场算法来计算表达式的值
int Calculator::doCalculator(vector<Token> tokens){
    stack<string> symbols; //符号栈
    stack<Num> result; //数字栈（即结果）
    prehandlerTokens(tokens);
    for(int i = 0; i < tokens.size(); i++){
        if(find(allSymbols.begin(), allSymbols.end(), tokens[i].content) != allSymbols.end()){
            if(symbols.empty() || priority.at(tokens.at(i).content) > priority.at(symbols.top())){
                symbols.push(tokens.at(i).content); //如果当前符号的优先级大于符号栈顶端的符号的优先级，就push进去
            }else{
                while(!symbols.empty() && priority.at(tokens.at(i).content) <= priority.at(symbols.top())){
                    result.push(calculate(result, symbols.top()));
                    symbols.pop(); //如果当前符号优先级不大于符号栈顶端符号优先级，就开始pop并计算值，直到空栈或者优先级大于，再push进去
                }
                symbols.push(tokens.at(i).content);
            }
            
        }
        else{
            if(tokens[i].describe.compare("identf") == 0){ //如果是一个变量，就从MemoryStack中读出变量的值，再push到数字栈中
                result.push(Num(tokens[i].content, memoryStack->getVariable(tokens[i].content)));
            }else if(tokens[i].content.compare("__FLAG__") == 0){ //如果是自增自减的标志，就push一个标志进去
                result.push(Num());
            }else{
                result.push(Num(atoi(tokens[i].content.c_str()))); //如果是常量，就将常量转为int再push进去
            }
        }
    }
    while(!symbols.empty()){ //如果操作已经结束了，然而符号栈还没有空，那么就将符号栈依次pop并计算即可
        result.push(calculate(result, symbols.top()));
        symbols.pop();
    }
    return result.top().num; //返回数字栈顶端的值
}

void Calculator::initialPriority(){
    allSymbols.insert(allSymbols.end(), "++");
    allSymbols.insert(allSymbols.end(), "--");
    allSymbols.insert(allSymbols.end(), "*");
    allSymbols.insert(allSymbols.end(), "/");
    allSymbols.insert(allSymbols.end(), "+");
    allSymbols.insert(allSymbols.end(), "-");
    allSymbols.insert(allSymbols.end(), ">=");
    allSymbols.insert(allSymbols.end(), "<=");
    allSymbols.insert(allSymbols.end(), ">");
    allSymbols.insert(allSymbols.end(), "<");
    allSymbols.insert(allSymbols.end(), "!=");
    allSymbols.insert(allSymbols.end(), "==");
    priority.insert(pair<string, int>("++", 5));
    priority.insert(pair<string, int>("--", 5));
    priority.insert(pair<string, int>("*", 4));
    priority.insert(pair<string, int>("/", 4));
    priority.insert(pair<string, int>("+", 3));
    priority.insert(pair<string, int>("-", 3));
    priority.insert(pair<string, int>(">=", 2));
    priority.insert(pair<string, int>("<=", 2));
    priority.insert(pair<string, int>(">", 2));
    priority.insert(pair<string, int>("<", 2));
    priority.insert(pair<string, int>("==", 1));
    priority.insert(pair<string, int>("!=", 1));
}

//pop出数字栈顶端的值以及符号栈顶端的一个符号，计算之后再push进去
Num Calculator::calculate(stack<Num> & result, string symbol){
    Num a = result.top();
    result.pop();
    Num b = result.top();
    result.pop();
    Num variable = b;
    if(symbol.compare("++") == 0){
        bool isA = false;
        if(b.name.compare("__FLAG__") == 0){
            isA = true;
            variable = a;
        }
        //处理++所导致的表达式副作用
        if(side_effect.find(variable.name) != side_effect.end()){
            int temp = side_effect.at(variable.name);
            side_effect.erase(variable.name);
            side_effect.insert(pair<string, int>(variable.name, temp + 1));
        }else{
            side_effect.insert(pair<string, int>(variable.name, 1));
        }
        return (isA) ? variable.num + 1 : variable.num;
    }else if(symbol.compare("--") == 0){
        bool isA = false;
        if(b.name.compare("__FLAG__") == 0){
            isA = true;
            variable = a;
        }
        //处理--所导致的表达式副作用
        if(side_effect.find(variable.name) != side_effect.end()){
            int temp = side_effect.at(variable.name);
            side_effect.erase(variable.name);
            side_effect.insert(pair<string, int>(variable.name, temp - 1));
        }else{
            side_effect.insert(pair<string, int>(variable.name, -1));
        }
        return (isA) ? variable.num - 1 : variable.num;
    }else if(symbol.compare("*") == 0){
        return Num(b.num * a.num);
    }else if(symbol.compare("/") == 0){
        return Num(b.num / a.num);
    }else if(symbol.compare("+") == 0){
        return Num(b.num + a.num);
    }else if(symbol.compare("-") == 0){
        return Num(b.num - a.num);
    }else if(symbol.compare(">=") == 0){
        return Num((int)(b.num >= a.num));
    }else if(symbol.compare("<=") == 0){
        return Num((int)(b.num <= a.num));
    }else if(symbol.compare("<") == 0){
        return Num((int)(b.num < a.num));
    }else if(symbol.compare(">") == 0){
        return Num((int)(b.num > a.num));
    }else if(symbol.compare("!=") == 0){
        return Num((int)(b.num != a.num));
    }else if(symbol.compare("==") == 0){
        return Num((int)(b.num == a.num));
    }
    return 0;
}

void Calculator::prehandlerTokens(vector<Token> & tokens){
    for(int i = 0; i < tokens.size(); i++){
        //如果出现了++\--的符号，就在其与另一个符号之间插入一个flag标志，将其变成一个形式上的双目运算符
        if(tokens.at(i).content.compare("++") == 0 || tokens.at(i).content.compare("--") == 0){
            if(i == 0 || find(allSymbols.begin(), allSymbols.end(), tokens[i - 1].content) != allSymbols.end()){
                tokens.insert(tokens.begin() + i, Token("__FLAG__", "Constant", -1, -1, -1));
            }else{
                tokens.insert(tokens.begin() + i + 1, Token("__FLAG__", "Constant", -1, -1, -1));
            }
            i++;
        }
        //如果出现了出现在变量前面的负号，就从MemoryStack中读出相应的结果，清除负号以及变量的Token，然后再将读出的结果加上负号再插入进去
        else if(tokens.at(i).content.compare("-") == 0){
            if(i == 0 || (tokens.at(i - 1).describe.compare("operator") == 0)){
                Token tempVar = tokens[i + 1];
                char temp[100];
                int value = memoryStack->getVariable(tempVar.content);
                value = -value;
                sprintf(temp, "%d", value);
                string valueString = string(temp);
                tempVar.content = valueString;
                tempVar.describe = "constant";
                tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
                tokens.insert(tokens.begin() + i, tempVar);
            }
        }
    }
}

void Calculator::cleanEffect(){
    side_effect.clear();
}

Num::Num(string name, int num){
    this->name = name;
    this->num = num;
}

Num::Num(int num){
    this->name = "__NULL__";
    this->num = num;
}

Num::Num(){
    this->name = "__FLAG__";
    this->num = -1;
}

