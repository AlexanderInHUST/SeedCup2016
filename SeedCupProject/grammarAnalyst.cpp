//
//  grammarAnalyst.cpp
//  SeedCupProject
//
//  Created by 唐艺峰，王启萌，朱一帆 on 16/10/29.
//  Copyright © 2016年 唐艺峰，王启萌，朱一帆. All rights reserved.
//

#include "grammarAnalyst.hpp"

GrammarAnalyst::GrammarAnalyst():calculator(&memoryStack){
    calculator = Calculator(&memoryStack); //将这个类中的MemoryStack的地址传给Calculator，使其能够获取变量的值
}

int GrammarAnalyst::analyse(vector<Token> * tokens){
    int breakFlag;
    IteratorManager manager(tokens);
    newVariable.push(vector<string>()); //push新的一层stack，存储当前作用域变量的变量名
    for(manager.jumpTo(tokens->begin()); manager.getIt() != tokens->end(); ){
        breakFlag = handleCurrentIt(&manager);
        if(breakFlag == BREAK_END) //返回了BREAK_END，不处理地将其继续传上去
            return breakFlag;
    }
    cleanNewVariable(); //分析完之后，清理当前作用域的所有变量
    return NORMAL_END;
}

int GrammarAnalyst::handleCurrentIt(IteratorManager * manager){
    if(manager->getIt()->describe.compare("keyword") == 0){
        if(manager->getIt()->content.compare("while") == 0){
            return handleWhile(manager);
        }
        else if(manager->getIt()->content.compare("if") == 0){
            return handleIf(manager);
        }
        else if(manager->getIt()->content.compare("for") == 0){
            return handleFor(manager);
        }
        else if(manager->getIt()->content.compare("int") == 0){
            return handleInt(manager);
        }
        else if(manager->getIt()->content.compare("do") == 0){
            return handleDo(manager);
        }
        else if(manager->getIt()->content.compare("break") == 0){ //遇见break语句之后，清理当前作用域所有变量，然后返回BREAK_END
            for(int i = 0; i < newVariable.top().size(); i++){
                memoryStack.popVariable(newVariable.top()[i]);
            }
            newVariable.pop();
            manager->move(1);
            return BREAK_END;
        }
    }
    else if(manager->getIt()->describe.compare("identf") == 0){
        if(manager->getIt()->content.compare("printf") == 0){
            return handlePrintf(manager);
        }
        else if((manager->getIt() + 2)->describe.compare("boundary") == 0){
            return handleSelfChange(manager);
        }
        else{
            return handleAssignment(manager);
        }
    }else if(manager->getIt()->describe.compare("operator") == 0){
        return handleSelfChange(manager);
    }
    else if(manager->getIt()->describe.compare("note") == 0){
        manager->jump(1);
    }
    else if(manager->getIt()->describe.compare("boundary") == 0){
        manager->jump(1);
    }
    return NORMAL_END;
}

int GrammarAnalyst::handleSelfChange(IteratorManager * manager){
    handleExpression(manager); //直接当做一个表达式进行处理即可
    manager->jump(1);
    return NORMAL_END;
}

int GrammarAnalyst::handleAssignment(IteratorManager * manager){
    bool isLast = true;
    vector<Token>::iterator temp = manager->getIt();
    do{ //判断当前是不是已经到达了类似于a = b = c = d;这样的赋值式的最后一位
        manager->jump(1);
        if(manager->getIt()->content.compare("=") == 0)
            isLast = false;
    }while(manager->getIt()->content.compare(";") != 0 && manager->getIt()->content.compare(",") != 0 );
    manager->jumpTo(temp);
    if(isLast){ //如果是的话，直接返回最后处理当前视为表达式的右值的结果
        int value = handleExpression(manager);
        manager->jump(1);
        return value;
    }
    else{ //如果不是的话，递归地计算下一个=之后的值，然后返回计算出来的值
        string name = manager->getIt()->content;
        manager->move(2);
        int value = handleAssignment(manager);
        memoryStack.setVariable(name, value);
        return value;
    }
}

int GrammarAnalyst::handleInt(IteratorManager * manager){
    vector<string> * variableInState = &newVariable.top();
    manager->jump(1);
    while(manager->getIt()->describe.compare("identf") == 0){
        variableInState->insert(variableInState->end(), manager->getIt()->content);
        memoryStack.pushVariable(manager->getIt()->content, 0);
        if((manager->getIt() + 1)->content == ";"){ //如果是int a; 直接结束
            manager->jump(2);
            break;
        }
        else if((manager->getIt() + 1)->content == "="){ //如果是 int a = 3; 将后面的 a = 3视为一个赋值式，直接调用处理赋值运算的方法
            handleAssignment(manager);
            if((manager->getIt() - 1)->content == ";")
                break;
        }
        else if((manager->getIt() + 1)->content == ","){ //如果是 int a , b, c; 跳到下一个变量的名字
            manager->jump(2);
        }
    }
    return NORMAL_END;
}

int GrammarAnalyst::handleIf(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfIfBorder = false;
    bool expression;
    vector<Token>::iterator startOfIf = manager->getIt(); // if语句块的起点
    vector<Token>::iterator endOfIt; // if语句块的终点的下一位Token
    
    endOfIt = getTheEndOfBlock(manager);
    manager->jumpTo(startOfIf);
    
    manager->jump(2);
    expression = (handleExpression(manager) == 0) ? false : true; // 计算表达式的结果
    manager->jump(1);
    if(manager->getIt()->content == "{"){ //判断if之后是否有{
        flagOfIfBorder = true;
    }
    if(!flagOfIfBorder){
        if(expression){
            vector<Token>::iterator startOfTemp = manager->getIt();
            vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
            for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                subTokens.insert(subTokens.end(), *it);
            }
            if(analyse(&subTokens) == BREAK_END) //如果if后面没有{，并且表达式为true，就分析处理下一个语句块
                return BREAK_END;
        }
        else{ //如果if后面没有{，并且表达式为false
            manager->jumpTo(getTheEndOfBlock(manager)); //跳过下一个语句块
            if(manager->getIt()->content.compare("else") == 0){ //如果有else的话，执行else中的内容
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    vector<Token>::iterator startOfTemp = manager->getIt();
                    vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
                    for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                        subTokens.insert(subTokens.end(), *it);
                    }
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
                else{ //else之后有{，将两个{}之间的代码重新截取出来，然后调用主分析方法，对截取部分进行递归分析
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    manager->jump(1);
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
            }
        }
    }else{
        if(!expression){ //如果if后面有{，并且表达式为false
            manager->jumpTo(getTheEndOfBracket(manager));
            
            if((manager->getIt() != manager->getEnd()) && manager->getIt()->content.compare("else") == 0){ //如果有else的话
                manager->jump(1);
                if(manager->getIt()->content.compare("{") != 0){
                    vector<Token>::iterator startOfTemp = manager->getIt();
                    vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
                    for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                        subTokens.insert(subTokens.end(), *it);
                    }
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
                else{
                    manager->jump(1);
                    while(!(manager->getIt()->content == "}" && stack == 0)){
                        if(manager->getIt()->content == "{")
                            stack++;
                        else if(manager->getIt()->content == "}")
                            stack--;
                        subTokens.insert(subTokens.end(), *manager->getIt());
                        manager->jump(1);
                    }
                    stack = 0;
                    manager->jump(1);
                    if(analyse(&subTokens) == BREAK_END)
                        return BREAK_END;
                }
            }
        }
        else{ //如果表达式为true，直接执行后面紧接着{}块
            while(!(manager->getIt()->content == "}" && stack == 1)){
                if(manager->getIt()->content == "{")
                    stack++;
                else if(manager->getIt()->content == "}")
                    stack--;
                subTokens.insert(subTokens.end(), *manager->getIt());
                manager->jump(1);
            }
            stack = 0;
            manager->jump(1);
            if(analyse(&subTokens) == BREAK_END)
                return BREAK_END;
        }
    }
    manager->jumpTo(endOfIt); //运行完之后，跳到if块的末尾
    return NORMAL_END;
}

int GrammarAnalyst::handleFor(IteratorManager * manager){
    int endFlag = NORMAL_END;
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    
    vector<Token>::iterator startOfFor = manager->getIt(); // for语句块的起点
    vector<Token>::iterator endOfFor; // for语句块的终点
    endOfFor = getTheEndOfBlock(manager);
    manager->jumpTo(startOfFor);
    
    vector<Token>::iterator startOfCondition; // for中间条件判断的起点
    vector<Token>::iterator startOfThirdExp; // for中第三个表达式的起点
    vector<Token>::iterator startOfContent; // for的执行代码块的起点
    while(manager->getIt()->content.compare(";") != 0)
        manager->jump(1);
    manager->jump(1);
    startOfCondition = manager->getIt();
    
    while(manager->getIt()->content.compare(";") != 0)
        manager->jump(1);
    manager->jump(1);
    startOfThirdExp = manager->getIt();
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    startOfContent = manager->getIt() + 1;
    manager->jumpTo(startOfFor);
    
    newVariable.push(vector<string>());
    manager->move(2);
    for(handleExpressionInFor(manager); manager->jumpTo(startOfCondition), handleExpression(manager);
        manager->jumpTo(startOfThirdExp),handleExpressionInFor(manager)){ // 把相应的处理带入
        Util::getResult(startOfFor->line);
        manager->jumpTo(startOfContent);
        if(manager->getIt()->content == "{"){
            flagOfBorder = true;
            manager->jump(1);
        }
        if(!flagOfBorder){
            if(!flagOfRead){
                vector<Token>::iterator startOfTemp = manager->getIt();
                vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
                for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                    subTokens.insert(subTokens.end(), *it);
                }
                flagOfRead = true;
            }
            endFlag = analyse(&subTokens);
            if(endFlag == BREAK_END)
                break;
        }
        else {
            if(!flagOfRead){ // 如果已经读取了token流的一段了，就不必重复读取
                while(!(manager->getIt()->content == "}" && stack == 0)){
                    if(manager->getIt()->content == "{")
                        stack++;
                    else if(manager->getIt()->content == "}")
                        stack--;
                    subTokens.insert(subTokens.end(), *manager->getIt());
                    manager->jump(1);
                }
                flagOfRead = true;
            }
            endFlag = analyse(&subTokens);
            if(endFlag == BREAK_END)
                break;
            Util::getResult((startOfFor + 2)->line);
        }
        manager->jumpTo(startOfFor + 2);
        
    }
    if(endFlag == NORMAL_END)
        Util::getResult(startOfFor->line);
    manager->jumpTo(endOfFor);
    cleanNewVariable();
    return NORMAL_END; // 所有作用域之中的BREAK_END信息在这里被拦截下来，之后继续返回NORMAL_END，以控制break只跳出当前循环
}

int GrammarAnalyst::handleExpressionInFor(IteratorManager * manager){ // 单独处理for条件中会出现的逗号表达式
    while(true){
        handleCurrentIt(manager);
        if((manager->getIt() - 1)->content.compare(";") == 0 || (manager->getIt() - 1)->content.compare(")") == 0)
            break;
    }
    return NORMAL_END;
}

int GrammarAnalyst::handleWhile(IteratorManager * manager){
    int endFlag = NORMAL_END;
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    
    vector<Token>::iterator startOfWhile = manager->getIt(); // while语句块的起点
    vector<Token>::iterator endOfWhile; //while语句块的终点
    endOfWhile = getTheEndOfBlock(manager);
    manager->jumpTo(startOfWhile);
    
    manager->move(2);
    while(handleExpression(manager)){ // 每次判断条件是否符合
        Util::getResult(startOfWhile->line);
        manager->jump(1);
        if(manager->getIt()->content == "{"){
            flagOfBorder = true;
            manager->jump(1);
        }
        if(!flagOfBorder){
            if(!flagOfRead){
                vector<Token>::iterator startOfTemp = manager->getIt();
                vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
                for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                    subTokens.insert(subTokens.end(), *it);
                }
                flagOfRead = true;
            }
            endFlag = analyse(&subTokens);
            if(endFlag == BREAK_END)
                break;
        }
        else {
            if(!flagOfRead){ // 如果已经读取了token流的一段了，就不必重复读取
                while(!(manager->getIt()->content == "}" && stack == 0)){
                    if(manager->getIt()->content == "{")
                        stack++;
                    else if(manager->getIt()->content == "}")
                        stack--;
                    subTokens.insert(subTokens.end(), *manager->getIt());
                    manager->jump(1);
                }
                flagOfRead = true;
            }
            endFlag = analyse(&subTokens);
            if(endFlag == BREAK_END)
                break;
            Util::getResult((startOfWhile + 2)->line);
        }
        manager->jumpTo(startOfWhile + 2);
        
    }
    if(endFlag == NORMAL_END)
        Util::getResult(startOfWhile->line);
    manager->jumpTo(endOfWhile);
    return NORMAL_END; // 所有作用域之中的BREAK_END信息在这里被拦截下来，之后继续返回NORMAL_END，以控制break只跳出当前循环
}

int GrammarAnalyst::handleDo(IteratorManager * manager){
    vector<Token> subTokens;
    int stack = 0;
    bool flagOfBorder = false;
    bool flagOfRead = false;
    
    vector<Token>::iterator startOfDo = manager->getIt(); // do-while语句块的起点
    vector<Token>::iterator endOfDo; // do-while语句块的终点
    vector<Token>::iterator startOfCondition; // do-while语句块的条件判断起点
    endOfDo = getTheEndOfBlock(manager);
    manager->jumpTo(startOfDo);
    
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        manager->jumpTo(getTheEndOfBracket(manager));
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    startOfCondition = manager->getIt() + 2;

    do{
        manager->jumpTo(startOfDo + 1);
        if(!flagOfRead){
            if(manager->getIt()->content.compare("{") == 0){
                flagOfBorder = true;
                manager->jump(1);
                while(!(manager->getIt()->content == "}" && stack == 0)){
                    if(manager->getIt()->content == "{")
                        stack++;
                    else if(manager->getIt()->content == "}")
                        stack--;
                    subTokens.insert(subTokens.end(), *manager->getIt());
                    manager->jump(1);
                }
                flagOfRead = true;
            }
        }
        if(flagOfBorder){
            if(analyse(&subTokens) == BREAK_END)
                break;
            Util::getResult((startOfCondition)->line);
        }
        else{
            vector<Token>::iterator startOfTemp = manager->getIt();
            vector<Token>::iterator endOfTemp = getTheEndOfBlock(manager);
            for(vector<Token>::iterator it = startOfTemp; it != endOfTemp; it++){
                subTokens.insert(subTokens.end(), *it);
            }
            if(analyse(&subTokens) == BREAK_END)
                break;
        }
    }while(manager->jumpTo(startOfCondition), handleExpression(manager));
    manager->jumpTo(endOfDo);
    return NORMAL_END;  // 所有作用域之中的BREAK_END信息在这里被拦截下来，之后继续返回NORMAL_END，以控制break只跳出当前循环
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfWhileOrFor(IteratorManager * manager){
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        manager->jumpTo(getTheEndOfBracket(manager));
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    return manager->getIt();
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfIf(IteratorManager * manager){
    while(manager->getIt()->content.compare(")") != 0)
        manager->jump(1);
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        manager->jumpTo(getTheEndOfBracket(manager));
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    while((manager->getIt() != manager->getEnd()) && (manager->getIt()->content.compare("else") == 0)){
        manager->jump(1);
        if(manager->getIt()->content.compare("if") == 0){
            while(manager->getIt()->content.compare(")") != 0)
                manager->jump(1);
            manager->jump(1);
        }
        if(manager->getIt()->content.compare("{") == 0){
             manager->jumpTo(getTheEndOfBracket(manager));
        }
        else{
            manager->jumpTo(getTheEndOfBlock(manager));
        }
    }
    return manager->getIt();
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfDo(IteratorManager * manager){
    manager->jump(1);
    if(manager->getIt()->content.compare("{") == 0){
        manager->jumpTo(getTheEndOfBracket(manager));
    }
    else{
        manager->jumpTo(getTheEndOfBlock(manager));
    }
    while(manager->getIt()->content.compare(";") != 0){
        manager->jump(1);
    }
    return manager->getIt() + 1;
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfBracket(IteratorManager * manager){
    int stack = 0;
    while(!(manager->getIt()->content == "}" && stack == 1)){
        if(manager->getIt()->content == "{")
            stack++;
        else if(manager->getIt()->content == "}")
            stack--;
        manager->jump(1);
    }
    manager->jump(1);
    return manager->getIt();
}

vector<Token>::iterator GrammarAnalyst::getTheEndOfBlock(IteratorManager * manager){
    if(manager->getIt()->content.compare("if") == 0){
        return getTheEndOfIf(manager);
    }
    else if(manager->getIt()->content.compare("for") == 0 || manager->getIt()->content.compare("while") == 0){
        return getTheEndOfWhileOrFor(manager);
    }
    else if(manager->getIt()->content.compare("do") == 0){
        return getTheEndOfDo(manager);
    }
    else{
        while(manager->getIt()->content.compare(";") != 0)
            manager->jump(1);
        return manager->getIt() + 1;
    }
}

int GrammarAnalyst::handlePrintf(IteratorManager * manager){
    manager->move(5);
    while(manager->getIt()->content == ","){ // 如果是,说明后面出现表达式
        manager->move(1);
        handleExpression(manager);
    }
    manager->jump(2);
    return NORMAL_END;
}

int GrammarAnalyst::handleExpression(IteratorManager * manager){ //将表达式截取下来，调用Calculator计算表达式的方法即可
    vector<Token> expression;
    int value;
    while(manager->getIt()->describe != "boundary"){
        expression.insert(expression.end(), *manager->getIt());
        manager->move(1);
    }
    if(expression.empty())
        return 1;
    else{
        value =  calculator.doCalculator(expression);
        memoryStack.handleSideEffect(calculator.side_effect);
        calculator.cleanEffect();
        return value;
    }
}

void GrammarAnalyst::cleanNewVariable(){ //清除作用域内的变量
    for(int i = 0 ; i < newVariable.top().size(); i++){
        memoryStack.popVariable(newVariable.top()[i]);
    }
    newVariable.pop();
}
