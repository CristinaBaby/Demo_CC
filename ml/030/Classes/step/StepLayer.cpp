//
//  StepLayer.cpp
//  MLABS1047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/4.
//
//

#include "StepLayer.h"
StepLayer::StepLayer()
{
    
}
StepLayer::~StepLayer()
{
    
}
bool StepLayer::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
    
}
void StepLayer::onEnter()
{
    GameBaseScene::onEnter();
}
void StepLayer::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void StepLayer::onExit()
{
    GameBaseScene::onExit();
}
void StepLayer::initUI()
{
    
    
    m_pBG = Sprite::create("images/bg/default_bg.png");
    kAdapterScreen->setExactPosition(m_pBG, 480, 320);
    this->addToBGLayer(m_pBG);
    
    //    showNextButton(true);
    showNextButton(true);
    
    
}
void StepLayer::nextStep()
{
    log("next");
    hideNextButton();
  
}