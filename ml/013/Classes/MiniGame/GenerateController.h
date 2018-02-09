//
//  GenerateController.h
//
//  Created by tanshoumei on 15/5/28.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "MathUtil.h"
using namespace cocos2d;

enum GenerateMode
{
    KGenerateDefault,
    KGenerateItemUnique,
};

class GenerateController;

class GenerateStrategy:public Ref
{
public:
    virtual void start() = 0;
    
    virtual void pause(){};
    
    virtual void resume(){};
    
    virtual void stop(){};
    
    void setGenerateController(GenerateController *aGenerateController);
    
protected:
    GenerateController *_generateController = nullptr;
};

//随时间变化策略
class GenerateStrategyTime: public GenerateStrategy
{
public:
    ~GenerateStrategyTime();
    
    GenerateStrategyTime(GenerateController *aGenerateController);
    
    virtual void start();
    
    virtual void pause();
    
    virtual void resume();
    
    virtual void stop();
    
    void update(float);
    
private:
    float _timePassed = 0;
};

/*
 物品生成控制器
 */

class GenerateController
{
public:
    ~GenerateController();
    
    //物品生成时间间隔
    void setGenerateDuration(float aDuration);
    
    //物品总类型数
    void setGenerateKindCount(const int aKindCount);
    
    //生成策略
    void setGenerateStrategy(GenerateStrategy * aGenerateStrategy);
    
    //生成道具的模式
    void setGenerateMode(GenerateMode kMode);
    
    void checkItemUnique(int idx);
    

    //开始
    void start();
    
    //停止
    void stop();
    
    //暂停
    void pause();
    
    //
    void resume();
    
    //
    void update(float dt);
    
    //
    int getGenerateNumber();
    
private:
    void _generate(GenerateMode kMode);
    
public:
    /*
     物品生成回调
     kind: 种类，取值[0,(KindCout - 1)]
     */
    std::function<void(const int kind)> didGeneratedCallback = nullptr;
    std::function<void(Ref* obj)> gameWin = nullptr;
    
private:
    GenerateStrategy *_generateStrategy = nullptr;
    //
    float _duration = 1.0f;
    //种类数
    int _kindCount = 0;
    //记录从上一次生成后过的时间
    float _timeDelta = 0;
    //
    int _generatedNumber = 0;
    //
    bool _pause = true;
    //
    GenerateMode mode_ = KGenerateDefault;
    vector<int> vec_ItemCount;
};

