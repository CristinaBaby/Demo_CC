//
//  MakeOtherScene.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/12.
//
//

#include "MakeOtherScene.h"
#include "MakeOtherStep1.h"
#include "MakeOtherStep2.h"
#include "MakeOtherScene2.h"

REGIST_SECENE(MakeOtherScene)
MakeOtherScene::MakeOtherScene()
{
    
}
MakeOtherScene::~MakeOtherScene()
{
    
}
bool MakeOtherScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    return true;
}
void MakeOtherScene::onEnter()
{
    Layer::onEnter();
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("gameBg.mp3");
    
    showLayer1();
}
void MakeOtherScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void MakeOtherScene::onExit()
{
    Layer::onExit();
}
void MakeOtherScene::showLayer1()
{
    MakeOtherStep1* step1 = MakeOtherStep1::create();
    this->addChild(step1);
    step1->nextStep=[=](){
        step1->removeFromParent();
        //进入下一个
        showLayer2(nullptr);
        
    };
}
void MakeOtherScene::showLayer2(Texture2D* spriteTexture2D)
{
    MakeOtherStep2* step2 = MakeOtherStep2::create();
    this->addChild(step2);
    step2->nextStep=[=](){
        step2->removeFromParent();
        //进入下一个
        showLayer3();
        
    };
    step2->resetLayer=[=](){
    
        step2->removeFromParent();
        showLayer2(nullptr);
    
    };
}
void MakeOtherScene::showLayer3()
{
    KitchenLayer::loading(kitchen_noodles,[=](){
    
        Director::getInstance()->runWithScene(TransitionFade::create(1.0,MakeOtherScene2::createScene()));
    
    });
}
void MakeOtherScene::showLayer4()
{
   
}