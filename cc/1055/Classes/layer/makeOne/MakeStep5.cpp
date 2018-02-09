//
//  MakeStep55.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/5.
//
//

#include "MakeStep5.h"
#include "ActionUtils.h"
#include "MakeStep6.h"

REGIST_SECENE(MakeStep5)

#define DataSpoonEmpty  1000
#define DataSpoonFull   1001

#define DataPlateFull   1002
#define DataPlatePush   1003

Vec2 spoonPos[4]={Vec2(523,313),Vec2(256,335),Vec2(280,500),Vec2(512,507)};

MakeStep5::MakeStep5()
{
    _spoonPos = Vec2::ZERO;
    m_pPushTips = nullptr;
    m_pMoveTips = nullptr;
    m_pHand = nullptr;
    m_pTitle = nullptr;
    m_bIsOver = false;
}
MakeStep5::~MakeStep5()
{
    
}
bool MakeStep5::init()
{
    if(!BaseLayer::init())
        return false;
    
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStep5.csb");
    addChild(_studioLayer);
    
    //加载碗，盘子一类的工具
    m_pBowl = _studioLayer->getNodeByName<Sprite*>("bowl");
    
    m_pScooper = _studioLayer->getNodeByName<Sprite*>("scooper");
    m_pScooper->setUserData((void*)DataSpoonEmpty);
    
    auto lisenter = MoveLisenter::create();
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pScooper);
    lisenter->setOrginOffset();
    lisenter->onTouchDown = CC_CALLBACK_2(MakeStep5::onTouchDown, this);
    lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeStep5::onTouchMoveInRect, this);
    lisenter->onTouchUp = CC_CALLBACK_2(MakeStep5::onTouchUp, this);

    m_pPlate = _studioLayer->getNodeByName<Sprite*>("plate");
    lisenter->addRectByNode(m_pBowl);
    
    for(int i = 0;i<4;i++){
        
        auto _rect = m_pPlate->getChildByName(__String::createWithFormat("rect%d",i)->getCString());
        
       
        Rect rect = RectApplyAffineTransform(Rect(0, 0, _rect->getContentSize().width, _rect->getContentSize().height),_rect->nodeToWorldTransform());
        rect.origin = rect.origin + Vec2(80,60);
        lisenter->addRect(rect);
        m_vPlateRect.pushBack(_rect);
    }
    
    
    auto flavo =  _studioLayer->getNodeByName<Sprite*>(GameDataInstance->getFlavor().c_str());
    if(flavo != nullptr)
        flavo->setVisible(true);
    
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");

    ActionHelper::showBackInOut(m_pScooper, 1.0, ActionHelper::ShowDirection::show_from_right,[=](){
    
        _spoonPos = m_pScooper->getPosition();
    
        if(!m_pHand){

            m_pHand = Sprite::create("cocostudio/UI/tips/finger.png");
            m_pHand->setPosition(_spoonPos + Vec2(100,0));
            this->addChild(m_pHand);
            
            m_pHand->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            
                m_pHand->setVisible(true);
            
            
            }),MoveTo::create(1.0,m_pBowl->getPosition()+ Vec2(100,0)),CallFunc::create([=](){
            
                m_pHand->setVisible(false);
                m_pHand->setPosition(_spoonPos + Vec2(100,0));
            
            
            }),DelayTime::create(1.0), NULL)));
        
        
        }
        if(!m_pMoveTips){
        
            m_pMoveTips = Sprite::create("cocostudio/UI/tips/jiantou.png");
            m_pMoveTips->setAnchorPoint(Vec2(0.62,0.21));
            m_pBowl->addChild(m_pMoveTips);
            m_pMoveTips->setPosition(Vec2(64,121));
            m_pMoveTips->setVisible(false);
            
            m_pMoveTips->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0),CallFunc::create([=](){
            
                m_pMoveTips->setVisible(true);
                
            }),RotateBy::create(0.5,-20),RotateBy::create(0.5,20),RotateBy::create(0.5,-20),RotateBy::create(0.5,20),CallFunc::create([=](){
            
                m_pMoveTips->setVisible(false);
            
            }), NULL)));
        }
    });
    
    ActionHelper::showBackInOut(m_pBowl, 0.8, ActionHelper::ShowDirection::show_from_right);
    ActionHelper::showBackInOut(m_pPlate, 0.5, ActionHelper::ShowDirection::show_from_left);
    
    if(!m_pTitle){
    
        m_pTitle = Sprite::create("cocostudio/UI/tips/scoop.png");
        m_pTitle->setAnchorPoint(Vec2(0.5,1));
        this->addChild(m_pTitle,100);
//        m_pTitle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height - 100 + 500));
        kAdapterScreen->setExactPosition(m_pTitle,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    }
    
    return true;
}
void MakeStep5::onEnter()
{
    BaseLayer::onEnter();
    if (m_pTitle) {
        
        m_pTitle->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(0.5, Vec2(0,-500)), 1.0), NULL));
        
        AudioHelp::getInstance()->playEffect("VoiceOver/scoop into the baking pan.mp3");
        
    }
}
void MakeStep5::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeStep5::onExit()
{
    BaseLayer::onExit();
}
void MakeStep5::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    if(this->getChildByTag(1003)){
    
        
        this->getChildByTag(1003)->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5, Vec2(0,500)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
            
            
        }), NULL));
    
    }
    
    
    if (m_pTitle) {
        
        m_pTitle->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(0.5, Vec2(0,500)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
            
            m_pTitle = nullptr;
            
        }), NULL));
        
    }
    
    if(m_pPushTips){
        m_pPushTips->removeFromParent();
        m_pPushTips = nullptr;
    }
    if(m_pHand){
        m_pHand->removeFromParent();
        m_pHand = nullptr;
    }
    if(m_pMoveTips){
        m_pMoveTips->removeFromParent();
        m_pMoveTips = nullptr;
    }
}
void MakeStep5::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    auto spoon = dynamic_cast<Sprite*>(node);
    if (index == 0) {
        
        //进入bowl
        size_t data = (size_t)spoon->getUserData();
        if (data == DataSpoonEmpty) {
        
            spoon->setTexture(__String::createWithFormat("cocostudio/Make4/scooper1_%s.png",GameDataInstance->getFlavor().c_str())->getCString());
            spoon->setUserData((void*)DataSpoonFull);
            
            AudioHelp::getInstance()->playEffect("button_general.mp3");
            auto flavo =  _studioLayer->getNodeByName<Sprite*>(GameDataInstance->getFlavor().c_str());
            if(flavo != nullptr){
            
                flavo->setScale(flavo->getScale()-0.2);
            
            }
            
            if(checkIsOver()==3)
                ActionUtils::moveByElasticIn(m_pBowl, Vec2(1000,0));
            
            
        }else if(data == DataSpoonFull){

            
        }
        
    }else if(index > 0){
    
        auto plateRect = m_vPlateRect.at(index-1);
        size_t data = (size_t)plateRect->getUserData();
        size_t _data = (size_t)spoon->getUserData();
        if(data!=DataPlateFull&&_data!=DataSpoonEmpty){
            
            lisenter->setEnabled(false);
            
            spoon->runAction(Sequence::create(MoveTo::create(0.5,spoonPos[index-1] + Vec2(-10,-20)),CallFunc::create([=](){
            
                spoon->setTexture(__String::createWithFormat("cocostudio/Make4/scooper2_%s.png",GameDataInstance->getFlavor().c_str())->getCString());
            
            }),RotateBy::create(0.25,30),CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("cookie scoop down.mp3");
                
            
            }),RotateBy::create(0.25,-30),CallFunc::create([=](){
            
               // AudioHelp::getInstance()->playEffect("cut.mp3");
            
            }),RotateBy::create(0.25,30),CallFunc::create([=](){
            
                AudioHelp::getInstance()->playEffect("cookie scoop down.mp3");
                
            
            }),RotateBy::create(0.25,-30),CallFunc::create([=](){
            
                spoon->setTexture("cocostudio/Make4/scooper3.png");
                
                auto plateCover = Sprite::create(__String::createWithFormat("cocostudio/Make4/plate_%s0.png",GameDataInstance->getFlavor().c_str())->getCString());
                plateCover->setPosition(plateRect->getContentSize()/2);
                plateRect->addChild(plateCover);
                plateCover->setScale(0.4);
                
                AudioHelp::getInstance()->playEffect("pour cookie.mp3");
                
                plateCover->runAction(Sequence::create(ScaleTo::create(0.5,0.85), NULL));
                plateRect->setUserData((void*)DataPlateFull);
                spoon->setUserData((void*)DataSpoonEmpty);
                
                if(checkIsOver()==4){
                    
                    lisenter->setEnabled(false);
                    ActionUtils::moveByElasticIn(spoon, Vec2(1000,0),1.0,CallFunc::create([=](){
                        
                        m_pScooper->removeFromParent();
                        overShow();
                        
                    }));
                }
                
            }),DelayTime::create(0.0),EaseElasticInOut::create(MoveTo::create(0.5,_spoonPos),1.0),CallFunc::create([=](){
                spoon->setTexture("cocostudio/Make4/scooper0.png");
                lisenter->setEnabled(true);
            }), NULL));
        }
    }
}
void MakeStep5::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    
}
void MakeStep5::touchEnd(ui::Widget* widget)
{
}
int MakeStep5::checkIsOver()
{
    int  num = 0;
    for(auto plateCover : m_vPlateRect){
    
        size_t data = (size_t)plateCover->getUserData();
        if(data == DataPlateFull)
            num++;
    }
    return num;
    
}
void MakeStep5::overShow()
{

    auto title = Sprite::create("cocostudio/UI/tips/presscookie.png");
    title->setAnchorPoint(Vec2(0.5,1));
    this->addChild(title,100);
    title->setTag(1003);
    //        m_pTitle->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height - 100 + 500));
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    title->runAction(Sequence::create(MoveBy::create(1.0,Vec2(0,-500)),CallFunc::create([=](){}), NULL));
    
    
    ActionUtils::moveToElasticOut(m_pPlate,m_pPlate->getParent()->getContentSize()/2,1.0,CallFunc::create([=](){
    
        AudioHelp::getInstance()->playEffect("VoiceOver/tap to press the cookies.mp3");
        
        for(auto rectPlate:m_vPlateRect){
        
            auto rect = Sprite::create(__String::createWithFormat("cocostudio/Make4/plate_%s0.png",GameDataInstance->getFlavor().c_str())->getCString());
            rect->setPosition(m_pPlate->convertToWorldSpace(rectPlate->getPosition()));
            this->addChild(rect);
            m_vPlateCookie.pushBack(rect);
            auto lisenter = MoveLisenter::create();
            
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, rect);
            lisenter->setOrginOffset();
            lisenter->onTouchDown = CC_CALLBACK_2(MakeStep5::onTouchDown, this);
            lisenter->onTouchMove = CC_CALLBACK_3(MakeStep5::onTouchMove, this);
            
            lisenter->m_bIsMoveBack = false;
            lisenter->m_bIsMove = false;
        }
        
        if(!m_pPushTips){
        
            m_pPushTips = Sprite::create("cocostudio/UI/tips/down.png");
            m_pPushTips->setPosition(Vec2(70,200));
            m_vPlateCookie.at(rand()%4)->addChild(m_pPushTips);
            
            m_pPushTips->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=](){
            
            
                m_pPushTips->setVisible(true);
            
            }),MoveBy::create(0.5, Vec2(0,-50)),CallFunc::create([=](){
                m_pPushTips->setVisible(false);
                
                m_pPushTips->setPositionY(m_pPushTips->getPositionY()+50);
                
            }),DelayTime::create(1.0), NULL)));
        
        }
    }));
}
void MakeStep5::onTouchMove(Node* node,MoveLisenter* lisenter,Touch* touch)
{

    Vec2 detalPos = touch->getDelta();
    if(detalPos.y<-10){
        
        auto rect = dynamic_cast<Sprite*>(node);
        size_t data = (size_t)rect->getUserData();
        if (data == DataPlatePush)
            return;
        
        AudioHelp::getInstance()->playEffect("press cookie.mp3");
        
        rect->setUserData((void*)DataPlatePush);
        rect->runAction(Sequence::create(ScaleTo::create(0.6, 1.2),CallFunc::create([=](){
        
            AudioHelp::getInstance()->playEffect("present tap.mp3");
            rect->setTexture(__String::createWithFormat("cocostudio/Make4/plate_%s1.png",GameDataInstance->getFlavor().c_str())->getCString());
            
            
            if(checkIsPushOver() == 4 && !m_bIsOver){
                m_bIsOver = true;
                
                ActionUtils::delayTimeAction(this,0.5,CallFunc::create([=](){
                
                    
                    FlowerLayer::Loading(this,[=](){
                        
//                        func_bind(nextStep);
                        KitchenLayer::loading(kitchen_oven,[=](){
                            
                            Director::getInstance()->runWithScene(TransitionFade::create(1.0,MakeStep6::createScene()));
                            //            Director::getInstance()->runWithScene(MakeStep6::createScene());
                            
                        });
                    });
                    
                }));
             }
        }), NULL));
    
    }
}
int MakeStep5::checkIsPushOver()
{
    int  num = 0;
    for(auto plateCover : m_vPlateCookie){
        
        size_t data = (size_t)plateCover->getUserData();
        if(data == DataPlatePush)
            num++;
    }
    return num;
    
}
