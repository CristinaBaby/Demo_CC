//
//  SugarManager.cpp
//  TOYBM0205
//
//  Created by huangwen on 15/10/23.
//
//

#include "SugarManager.hpp"
#include "UITools.h"
#include "Sugar.hpp"
#include "PensColorManager.h"

SugarManager* SugarManager::_instance = nullptr;

SugarManager* SugarManager::getInstance(){
    if (!_instance) {
        _instance = new (std::nothrow)SugarManager();
    }
    return _instance;
}

void SugarManager::start(){
    _visibleSize = Director::getInstance()->getVisibleSize();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void SugarManager::stop(){
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void SugarManager::update(float dt){
    _acc += dt;
    
    float limit = 0.08 + quickRandomFloat(0, 0.07);
    if (_acc > limit) {
        int index = quickRandom(2, 6);
        
        string filename = "sugar" + std::to_string(index) + ".png";
        auto sugar = Sprite::create("mm/" + filename);
        
        sugar->setScale(0.7f);
        
        Size sugarSize = sugar->getContentSize();
        
        auto pos = Vec2(quickRandomFloat(0.02, 0.98) * _visibleSize.width , _visibleSize.height + sugarSize.height/2);
        sugar->setPosition(pos);
        
        Scene *pRunScene = Director::getInstance()->getRunningScene();
        if (pRunScene) {
            auto saveLaye = pRunScene->getChildByName("saveLayer");
            saveLaye->addChild(sugar, 200);
        }
        
        //向顶部移动
        float duration = 3.0 + quickRandomFloat(0, 0.5);
        auto sequense = Sequence::create(MoveBy::create(duration, Vec2(0, -(_visibleSize.height/2 + sugarSize.height))),
                                         CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sugar)), NULL);
        auto angel = quickRandomFloat(90, 540);
        
        if (quickRandom(0, 1) == 1) {
            angel *= -1;
        }
        
        auto rotation = RotateBy::create(duration, angel);
        sugar->runAction(Spawn::create(rotation, sequense,NULL));
        
        _count ++;
        _acc = 0;
        if (_count > 70) {
            _count = 0;
            stop();
        }
    }
}

void SugarManager::destroy(){
    CC_SAFE_DELETE(_instance);
}

