//
//  enterMarket.cpp
//  MLABS030_IceCreamKids_CookingGames
//
//  Created by wusonglin1 on 15/9/6.
//
//

#include "EnterMarket.h"
EnterMarket::EnterMarket()
{
    
}
EnterMarket::~EnterMarket()
{
    
}
bool EnterMarket::init()
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
void EnterMarket::onEnter()
{
    GameBaseScene::onEnter();
    
    this->carAction(0);
    
    
}
void EnterMarket::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void EnterMarket::onExit()
{
    GameBaseScene::onExit();
    AudioHelp::getInstance()->stopLoofEffect();
}
void EnterMarket::initUI()
{
    m_pBG = Sprite::create("images/drive/truckdrive_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG, 0, 320,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addToBGLayer(m_pBG);
    
    m_pCar = Sprite::create("images/home/start_car.png");
    kAdapterScreen->setExactPosition(m_pCar, 0 - 500, 20,Vec2(0.5,0),kBorderLeft,kBorderBottom);
    this->addToContentLayer(m_pCar, 5);
    
    auto wheel = Sprite::create("images/home/start_tyre0.png");
    wheel->setPosition(Vec2(230,86));
    wheel->setScale(0.9);
    wheel->setTag(10);
    m_pCar->addChild(wheel);
    
    auto wheel1 = Sprite::create("images/home/start_tyre0.png");
    wheel1->setPosition(Vec2(579,80));
    wheel1->setScale(0.9);
    wheel1->setTag(20);
    m_pCar->addChild(wheel1);
    
    wheel->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));
    wheel1->runAction(RepeatForever::create(RotateBy::create(0.5, 120)));

    
    //    showNextButton(true);
//    showNextButton(true);
}

void EnterMarket::carAction(float dt)
{
    AudioHelp::getInstance()->playLoofEffect("6.truck.mp3");
    auto func = CallFunc::create([=](){
    
        bgAction(0);
        
    });
    
    MoveBy* move = MoveBy::create(5.0, Vec2(900,0));
    m_pCar->runAction(Sequence::create(move, func,NULL));
}
void EnterMarket::bgAction(float dt)
{
    m_pCar->getChildByTag(10)->stopAllActions();
    m_pCar->getChildByTag(20)->stopAllActions();
    
    m_pCar->getChildByTag(10)->runAction(RepeatForever::create(RotateBy::create(0.5, 300)));
    m_pCar->getChildByTag(20)->runAction(RepeatForever::create(RotateBy::create(0.5, 300)));
    
    auto func = CallFunc::create([=](){
        _carAction(0);
    });
    
    
    
    
    MoveBy* move = MoveBy::create(2.5, Vec2(-(m_pBG->getContentSize().width - Director::getInstance()->getVisibleSize().width),0));
    EaseSineOut* eso = EaseSineOut::create(move);
    m_pBG->runAction(Sequence::create(eso, func,NULL));
}
void EnterMarket::_carAction(float dt)
{
    m_pCar->getChildByTag(10)->stopAllActions();
    m_pCar->getChildByTag(20)->stopAllActions();
    
    m_pCar->getChildByTag(10)->runAction(RepeatForever::create(RotateBy::create(0.5, 100)));
    m_pCar->getChildByTag(20)->runAction(RepeatForever::create(RotateBy::create(0.5, 100)));
    
    auto func = CallFunc::create([=](){

        m_pCar->getChildByTag(10)->stopAllActions();
        m_pCar->getChildByTag(20)->stopAllActions();
        
        showNextButton(true);
        AudioHelp::getInstance()->stopLoofEffect();
    });
    
    MoveBy* move = MoveBy::create(2.0, Vec2(200,0));
    m_pCar->runAction(Sequence::create(move, func,NULL));
    
}
void EnterMarket::nextStep()
{
    log("next");
    hideNextButton();
    
    SceneChangeManager->enterMarket();
    
}