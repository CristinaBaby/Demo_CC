//
//  LoadingScene.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-18.
//
//

#include "LoadingScene.h"
#include "AudioHelp.h"
#include "AdapterScreen.h"
#include "Global.h"
#include "SSCInternalLibManager.h"
#include "ActionUtils.h"
#include "HomeScene.h"

bool LoadingScene::init()
{
    if ( !Scene::init() ) {
        return false;
    }
    this->loading();
    
    return true;
}

void LoadingScene::loading()
{
    
    LayerColor* contentLayer = LayerColor::create(Color4B(255, 125, 1,255));
    this->addChild(contentLayer);
    
 
    
    CallFunc *callBack = CCCallFunc::create(CC_CALLBACK_0(LoadingScene::callBack,this));
    //////////////////////////////////////////////////////////
    // oven animation
    //    auto pOvenAnimation = Animation::create();
    //
    //    for (int i = 1; i < 6; i++) {
    //
    //        pOvenAnimation->addSpriteFrameWithFile(__String::createWithFormat("images/background/%d.png", i)->getCString());
    //    }
    //
    //    pOvenAnimation->setDelayPerUnit(0.1f);
    //    pOvenAnimation->setLoops(4);
    //
    //    auto pOvenAnimat = Animate::create(pOvenAnimation);
    //
    //    pop->runAction(Sequence::create(pOvenAnimat, CCDelayTime::create(0.5),callBack,NULL));
    
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("loading/loading.plist");
    
    auto sprite3 = Sprite::createWithSpriteFrameName("001.png");
    kAdapterScreen->setExactPosition(sprite3, 480, 320);
    this->addChild(sprite3);
    
    Vector<cocos2d::SpriteFrame *> arr;
    char str[100] = {0};
    for (int i=1; i<10; i++)
    {
        sprintf(str, "00%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    for (int i=10; i<55; i++)
    {
        if (i==29) {
            continue;
        }
        sprintf(str, "0%d.png", i);
        auto frame_2 = cache->SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        arr.pushBack(frame_2);
    }
    
    
    
    auto animation3 = Animation::createWithSpriteFrames(arr, 0.1f);
    sprite3->runAction(Sequence::create(Animate::create(animation3),callBack,NULL));
    //    sprite3->runAction(RepeatForever::create(Animate::create(animation3)));
    
    
    auto func = CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("loading.mp3");
        
    });
    ActionUtils::delayTimeAction(this,0.3,func);
}
void LoadingScene::preLoad()
{
    AudioHelp::getInstance()->setAudioVolumeMode(true);
}
void LoadingScene::callBack()
{
//    SceneManager::getInstance()->enterHomeScene();
    Director::getInstance()->runWithScene(HomeScene::createScene());
//    CFSystemFunction sys;
//    sys.showNewsBlast(NewsModeLaunch);
    SSCInternalLibManager::getInstance()->doNewsBlast();
}
