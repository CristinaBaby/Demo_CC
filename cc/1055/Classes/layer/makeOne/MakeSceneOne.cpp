//
//  MakeSceneOne.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/3/31.
//
//

#include "MakeSceneOne.h"
#include "MakeStep1.h"
#include "MakeStep2.h"
#include "MakeStep3.h"
#include "MakeStep4.h"
#include "MakeStep5.h"

REGIST_SECENE(MakeSceneOne)
MakeSceneOne::MakeSceneOne()
{

}
MakeSceneOne::~MakeSceneOne()
{

}
bool MakeSceneOne::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    
    return true;
}
void MakeSceneOne::onEnter()
{
    Layer::onEnter();
    
    AudioHelp::getInstance()->StopBackgroundMusic();
    AudioHelp::getInstance()->playBackgroundMusic("gameBg.mp3");

    
    log("%s",FileUtils::getInstance()->getWritablePath().c_str());
    
    showLayer1();
//    showLayer3();
}
void MakeSceneOne::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
void MakeSceneOne::onExit()
{
    Layer::onExit();
}
void MakeSceneOne::showLayer1()
{
    MakeStep1* step1 = MakeStep1::create();
    this->addChild(step1);
    step1->nextStep=[=](){
        step1->removeFromParent();
        //进入下一个
        showLayer2(nullptr);
    };
}
void MakeSceneOne::showLayer2(Texture2D* texture)
{
    MakeStep2* step2 = MakeStep2::create();
   
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
void MakeSceneOne::showLayer3()
{
    MakeStep3* step3 = MakeStep3::create();
    this->addChild(step3);
    step3->nextStep=[=](){
        step3->removeFromParent();
        //进入下一个
        showLayer4(nullptr);
        
    };
}
void MakeSceneOne::showLayer4(Texture2D* texture)
{
    MakeStep4* step4 = MakeStep4::create();
    this->addChild(step4);
    step4->nextStep=[=](){
        step4->removeFromParent();
        //进入下一个
        
        KitchenLayer::loading(kitchen_noodles,[=](){
        
            Director::getInstance()->runWithScene(TransitionFade::create(1.0,MakeStep5::createScene()));
        
        });
    };
    step4->resetLayer=[=](){
        
        step4->removeFromParent();
        showLayer4(nullptr);
        
    };
}