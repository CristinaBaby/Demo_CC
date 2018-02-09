//
//  GenerateController.cpp
//
//  Created by tanshoumei on 15/5/28.
//
//

#include "GenerateController.h"

void GenerateStrategy::setGenerateController(GenerateController *aGenerateController)
{
    _generateController = aGenerateController;
}

GenerateStrategyTime::~GenerateStrategyTime()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

GenerateStrategyTime::GenerateStrategyTime(GenerateController *aGenerateController)
{
    _generateController = aGenerateController;
}

void GenerateStrategyTime::start()
{
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void GenerateStrategyTime::update(float dt)
{
    _timePassed += dt;
    if(_generateController)
    {
        //每10秒加速一点
        float duration = 1.0 - (int(_timePassed) / 5) * 0.1;
        //最小值0.5
        duration = fmax(duration, 0.3);
        _generateController->setGenerateDuration(duration);
    }
}

void GenerateStrategyTime::pause()
{
    Director::getInstance()->getScheduler()->pauseTarget(this);
}

void GenerateStrategyTime::resume()
{
    Director::getInstance()->getScheduler()->resumeTarget(this);
}

void GenerateStrategyTime::stop()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

GenerateController::~GenerateController()
{
    CC_SAFE_RELEASE_NULL(_generateStrategy);
}

void GenerateController::setGenerateDuration(float aDuration)
{
    _duration = aDuration;
}

void GenerateController::setGenerateKindCount(const int aKindCount)
{
    _kindCount = aKindCount;
}

void GenerateController::setGenerateMode(GenerateMode kMode)
{
    mode_ = kMode;

}

void GenerateController::setGenerateStrategy(GenerateStrategy *aGenerateStrategy)
{
    _generateStrategy = aGenerateStrategy;
    _generateStrategy->retain();
}

void GenerateController::start()
{
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    if(_generateStrategy){
        _generateStrategy->start();
    }
    _pause = false;
}

void GenerateController::stop()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    if(_generateStrategy){
        _generateStrategy->stop();
    }
    _pause = true;
}

void GenerateController::pause()
{
    Director::getInstance()->getScheduler()->pauseTarget(this);
    if(_generateStrategy){
        _generateStrategy->pause();
    }
    _pause = true;
}

void GenerateController::resume()
{
    Director::getInstance()->getScheduler()->resumeTarget(this);
    if(_generateStrategy){
        _generateStrategy->resume();
    }
    _pause = false;
}

void GenerateController::update(float dt)
{
    if (!_pause) {
        _timeDelta += dt;
        if(_timeDelta >= _duration)
        {
            _generate(mode_);
            _timeDelta = fmod(_timeDelta, _duration);
        }
    }
}

void GenerateController::_generate(GenerateMode kMode)
{
    switch (kMode)
    {
        case KGenerateDefault:
        {
        
            ++_generatedNumber;
            if(didGeneratedCallback){
                int idx = SM::MathUtil::randomBetween(1, _kindCount);
                didGeneratedCallback(idx);
            }
//            __CCLOGWITHFUNCTION("generate:%d", _generatedNumber);

        }
            break;
        case KGenerateItemUnique:
        {
            if(vec_ItemCount.size() == 0 )
            {
                for (int i = 1; i<_kindCount; i++)
                {
                    vec_ItemCount.push_back(i);
                }
                
            }
            

            
            ++_generatedNumber;
            if(didGeneratedCallback){
                int idx = SM::MathUtil::randomBetween(1, _kindCount);

                if (idx == 5)
                {
                    log("idxidxidxidxidxidx%d," ,idx);
                }
                
                if (idx == 6)
                {
                    log("idxidxidxidxidxid6666666666666x%d," ,idx);
                }
                
                if(idx > vec_ItemCount.size())
                {
                    didGeneratedCallback(idx);
                }
                else if(idx == vec_ItemCount.at(idx-1))
                {
                    didGeneratedCallback(idx);
                }
                else if(vec_ItemCount.at(idx-1) == 0)
                {
                    
                    //如果道具已经达成 则重新随机一个数
                    while (vec_ItemCount.at(idx-1) == 0)
                    {
                        idx = SM::MathUtil::randomBetween(1, _kindCount-1);
                    }

                    didGeneratedCallback(idx);
                }
            }
//            __CCLOGWITHFUNCTION("generate:%d", _generatedNumber);
            
        }
            break;
            
        default:
            break;
    }
    
}


void GenerateController::checkItemUnique(int idx)
{
    
    log("checkItemUnique%d," ,idx);

    
    if (vec_ItemCount.size()>0)
    {
        //将对应的道具设置为已经达成
        vec_ItemCount.at(idx-1) = 0;
    }
    for (int i=0; i<vec_ItemCount.size(); i++)
    {
        
        //如果全部为0 则说明已经达成胜利条件
        if (vec_ItemCount.at(i)!=0)
        {
            return;
        }
    }
    
    if (gameWin)
    {
        gameWin(nullptr);
        gameWin = nullptr;
    }
    
    
    this -> stop();

}


int GenerateController::getGenerateNumber()
{
    return _generatedNumber;
}