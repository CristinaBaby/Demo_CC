//
//  MakeOtherStep5.cpp
//  CCATS1054_CookieMaker
//
//  Created by wusonglin1 on 16/4/14.
//
//

#include "MakeOtherStep5.h"
#include "MakeStepDec.h"

REGIST_SECENE(MakeOtherStep5)

MakeOtherStep5::MakeOtherStep5()
{
    _zorder = 0;
    m_bIsTouchBtn = false;
    m_bIsOvenStart = false;
}
MakeOtherStep5::~MakeOtherStep5()
{
}
bool MakeOtherStep5::init()
{
    
    if(!BaseLayer::init())
        return false;
    //加载_studioLayer
    _studioLayer = StudioLayer::create("MakeStep6.csb");
    addChild(_studioLayer);
    
    m_pCookie  = _studioLayer->getNodeByName<Sprite*>("oven_cookie");
    
    auto lisenter = MoveLisenter::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, m_pCookie);
    lisenter->setOrginOffset();
    
    lisenter->onTouchDown       = CC_CALLBACK_2(MakeOtherStep5::onTouchDown, this);
    lisenter->onTouchMoveInRect = CC_CALLBACK_3(MakeOtherStep5::onTouchMoveInRect, this);
    lisenter->onTouchUp         = CC_CALLBACK_2(MakeOtherStep5::onTouchUp, this);
    
    m_pRectNode = _studioLayer->getNodeByName<Sprite*>("rect");
    lisenter->addRectByNode(m_pRectNode);
    
    m_pOvenDoorOpen  = _studioLayer->getNodeByName<Sprite*>("oven_open");
    
    m_pOvenDoorClose = _studioLayer->getNodeByName<Sprite*>("oven_close");
    
    m_pOvenDoorOpen->setVisible(false);
    m_pCookie->setPosition(m_pCookie->getPosition() + Vec2(-1000,0));
    _zorder = m_pCookie->getLocalZOrder();
    
    auto light = _studioLayer->getNodeByName<Sprite*>("light_green");
    light->setTag(100);
    auto lisenter_light = MoveLisenter::create();
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter_light, light);
    lisenter_light->setOrginOffset();
    lisenter_light->m_bIsMove = false;
    lisenter_light->onTouchDown       = CC_CALLBACK_2(MakeOtherStep5::onTouchDown, this);
    
    auto title = Sprite::create("cocostudio/UI/tips/bakecookie.png");
    kAdapterScreen->setExactPosition(title,480,20 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(title);
    title->setTag(1001);
    
    
    
    if(GameDataInstance->getMold() == MoldRound){
    
        m_pCookie->setTexture(__String::createWithFormat("cocostudio/Make5/oven_round_%scookie.png",GameDataInstance->getFlavor().c_str())->getCString());
        
    
    }else{
    
        m_pCookie->setTexture(__String::createWithFormat("cocostudio/Make5/oven_heart_%scookie.png",GameDataInstance->getFlavor().c_str())->getCString());
        
    }
    
    
    return true;
}
void MakeOtherStep5::onEnter()
{
    BaseLayer::onEnter();
    
    this->scheduleOnce(schedule_selector(MakeOtherStep5::startOven), 0.5);
}
void MakeOtherStep5::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
}
void MakeOtherStep5::onExit()
{
    BaseLayer::onExit();
}
void MakeOtherStep5::startOven(float dt)
{
    m_pOvenDoorOpen->setVisible(true);
    m_pOvenDoorClose->setVisible(false);
    
    AudioHelp::getInstance()->playEffect("ingredients_fly_in.mp3");
    
    m_pCookie->runAction(Sequence::create(EaseElasticOut::create(MoveBy::create(1.0,Vec2(1000,0)),1.0), CallFunc::create([=](){
        
        AudioHelp::getInstance()->playEffect("VoiceOver/let's bake the cookies.mp3");
        
        //showHand
        if (this->getChildByTag(1001)) {
            
            this->getChildByTag(1001)->runAction(MoveBy::create(0.5,Vec2(0,-500)));
            
        }
        
    }),NULL));
}
void MakeOtherStep5::onTouchDown(Node* node,MoveLisenter* lisenter)
{
    
    if (this->getChildByTag(1001)) {
        
        this->getChildByTag(1001)->runAction(EaseElasticIn::create(MoveBy::create(0.5,Vec2(0,500)),1.0));
        
    }
    
    int tag = node->getTag();
    if (tag == 100) {
        
        AudioHelp::getInstance()->playEffect("button_general.mp3");
        
        if (!m_bIsTouchBtn) {
            return;
        }
        if(!m_bIsOvenStart){
            
            m_bIsTouchBtn = false;
            
            _studioLayer->getNodeByName<Sprite*>("light_green")->setTexture("cocostudio/Make5/light_red.png");
            
            auto click = _studioLayer->getNodeByName<Sprite*>("click");
            auto hand  = _studioLayer->getNodeByName<Sprite*>("hand");
            click->setVisible(false);
            hand->setVisible(false);
            click->stopAllActions();
            hand->stopAllActions();
            
            auto fire = _studioLayer->getNodeByName<Sprite*>("oven_fire");
            fire->setVisible(true);
            
            auto pOvenAnimation = Animation::create();
            pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make5/oven_fire0.png");
            pOvenAnimation->addSpriteFrameWithFile("cocostudio/Make5/oven_fire1.png");
            pOvenAnimation->setDelayPerUnit(0.5);
            pOvenAnimation->setLoops(-1);
            auto pOvenAnimat = Animate::create(pOvenAnimation);
            fire->runAction(pOvenAnimat);
            
            AudioHelp::getInstance()->playLoofEffect("baking.mp3");
            
            ActionUtils::delayTimeAction(this,3.0,CallFunc::create([=](){
                
                m_bIsOvenStart = true;
                m_bIsTouchBtn  = true;
                showHand(0);
            }));
        }else{
            
            AudioHelp::getInstance()->stopLoofEffect();
            
            AudioHelp::getInstance()->playEffect("baking ok.mp3");
            
            m_bIsTouchBtn  = false;
            
            _studioLayer->getNodeByName<Sprite*>("light_green")->setTexture("cocostudio/Make5/light_green.png");
            
            auto click = _studioLayer->getNodeByName<Sprite*>("click");
            auto hand  = _studioLayer->getNodeByName<Sprite*>("hand");
            click->setVisible(false);
            hand->setVisible(false);
            click->stopAllActions();
            hand->stopAllActions();
            
            
            auto fire = _studioLayer->getNodeByName<Sprite*>("oven_fire");
            fire->setVisible(false);
            m_pOvenDoorOpen->setVisible(true);
            m_pOvenDoorClose->setVisible(false);
            
            ActionUtils::delayTimeAction(this,0.0,CallFunc::create([=](){
                
                m_pOven_cookie->setVisible(false);
                
                auto bg = _studioLayer->getNodeByName<Sprite*>("bg_10");
                auto plate = Sprite::create("cocostudio/Make5/plate.png");
                plate->setPosition(bg->convertToWorldSpace(m_pOvenDoorOpen->convertToWorldSpace(Vec2(408,242))));
                this->addChild(plate,100);
                
                auto plateCover = Sprite::create(__String::createWithFormat("cocostudio/MakeOther5/plate_%s_%s.png",GameDataInstance->getMold().c_str(),GameDataInstance->getFlavor().c_str())->getCString());
                plateCover->setPosition(plate->getContentSize()/2);
                plate->addChild(plateCover);
                
                plate->setScale(0.5);
                
                 AudioHelp::getInstance()->playEffect("slide.mp3");
                
                plate->runAction(Sequence::create(Spawn::create(MoveTo::create(1.0,plate->getParent()->getContentSize()/2),ScaleTo::create(1.0,1.0),RotateBy::create(1.0,360), NULL),CallFunc::create([=](){
                    
                    FlowerLayer::Loading(this,[=](){
                        
                        KitchenLayer::loading(kitchen_dec,[=](){
                            
                            Director::getInstance()->runWithScene(TransitionFade::create(1.0,MakeStepDec::createScene()));
                            
                        });
                    });
                    
                }), NULL));
            }));
        }
    }
}
void MakeOtherStep5::onTouchMoveInRect(Node* node,MoveLisenter* lisenter,int index)
{
    m_pOvenDoorClose->getParent()->reorderChild(m_pOvenDoorClose, 1001);
    lisenter->setEnabled(false);
    m_pCookie->setVisible(false);
    m_pCookie->runAction(Sequence::create(CallFunc::create([=](){
        
        m_pCookie->setVisible(false);
        m_pOven_cookie   = _studioLayer->getNodeByName<Sprite*>("oven_cookie_1");
        if(GameDataInstance->getMold() == MoldRound){
            
            m_pOven_cookie->setTexture(__String::createWithFormat("cocostudio/Make5/oven_round_%scookie.png",GameDataInstance->getFlavor().c_str())->getCString());
            
            
        }else{
            
            m_pOven_cookie->setTexture(__String::createWithFormat("cocostudio/Make5/oven_heart_%scookie.png",GameDataInstance->getFlavor().c_str())->getCString());
            
        }
        
        m_pOven_cookie->setVisible(true);
        
    }),DelayTime::create(1.0),CallFunc::create([=](){
        
        m_pOvenDoorOpen->setVisible(false);
        m_pOvenDoorClose->setVisible(true);
        
        showHand(0);
        
    }),DelayTime::create(3.0),CallFunc::create([=](){
        
    }),DelayTime::create(0.5),CallFunc::create([=](){
        
    }),NULL));
}
void MakeOtherStep5::showHand(float dt)
{
    auto click = _studioLayer->getNodeByName<Sprite*>("click");
    auto hand = _studioLayer->getNodeByName<Sprite*>("hand");
    hand->setPosition(Vec2(803,459));
    click->setVisible(true);
    hand->setVisible(true);
    
    click->getParent()->reorderChild(click, 1002);
    hand->getParent()->reorderChild(hand, 1002);
    click->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.0,0.8),ScaleTo::create(1.0,0.6),nullptr)));
    hand->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0,Vec2(0,70)),MoveBy::create(0.2,Vec2(0,-70)), NULL)));
    
    m_bIsTouchBtn = true;
}
void MakeOtherStep5::onTouchUp(Node* node,MoveLisenter* lisenter)
{
    
}
void MakeOtherStep5::touchEnd(ui::Widget* widget)
{
    if(!m_bIsTouchBtn)
        return;
    
    
    int tag = widget->getTag();
    if(tag == 100){
        
        
    }
    
}