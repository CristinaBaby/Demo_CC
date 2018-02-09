//
//  MakeOtherScene22.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#include "MakeOtherScene2.h"
#include "MakeOtherStep1.h"
#include "MakeOtherStep2.h"
#include "MakeOtherStep3.h"
#include "MakeOtherStep4.h"
#include "MakeOtherStep5.h"

REGIST_SECENE(MakeOtherScene2)
MakeOtherScene2::MakeOtherScene2()
{
    
}
MakeOtherScene2::~MakeOtherScene2()
{
    
}
bool MakeOtherScene2::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    
    
    
    return true;
}
void MakeOtherScene2::onEnter()
{
    Layer::onEnter();
    showLayer1();
}
void MakeOtherScene2::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void MakeOtherScene2::onExit()
{
    Layer::onExit();
}
void MakeOtherScene2::showLayer1()
{
    MakeOtherStep3* step1 = MakeOtherStep3::create();
    this->addChild(step1);
    step1->nextStep=[=](){
        step1->removeFromParent();
        //进入下一个
        showLayer2(nullptr);
        
    };
}
void MakeOtherScene2::showLayer2(Texture2D* spriteTexture2D)
{
    MakeOtherStep4* step2 = MakeOtherStep4::create();
    this->addChild(step2);
    step2->nextStep=[=](){
        step2->removeFromParent();
        //进入下一个
        showLayer3();
        
    };
}
void MakeOtherScene2::showLayer3()
{
    KitchenLayer::loading(kitchen_oven,[=](){
        
        Director::getInstance()->runWithScene(TransitionFade::create(1.0,MakeOtherStep5::createScene()));
        
    });
}
void MakeOtherScene2::showLayer4()
{
    
}