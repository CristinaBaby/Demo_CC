//
//  IcePopStepLayer3.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/6.
//
//

#include "IcePopStepLayer3.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"
#include "SimpleAudioEngine.h"
#include "IcePopDecorationLayer.h"

IcePopStepLayer3::IcePopStepLayer3(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IcePopStepLayer3::~IcePopStepLayer3()
{
    
}
IcePopStepLayer3* IcePopStepLayer3::create(GameLayer* gameLayer)
{
    IcePopStepLayer3* step1 = new IcePopStepLayer3(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IcePopStepLayer3::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        initData();
      
        CC_BREAK_IF(!initUI());
        isInit = true;
        
    } while (0);
    
    return isInit;
}
void IcePopStepLayer3::initData()
{
}
bool IcePopStepLayer3::initUI()
{
    Sprite* bg = Sprite::create("images/ice_pop/3/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
   
   
    
    m_pShadow = Sprite::create("images/ice_pop/3/refrigerator_shadow.png");
    m_pShadow->setAnchorPoint(Vec2(0.5,0.5));
    m_pShadow->setPosition(Vec2(384,128 + 120));
    m_pShadow->setScale(0.7);
    bg->addChild(m_pShadow);
    
    m_pRefrigerator = Sprite::create("images/ice_pop/3/refrigerator.png");
    m_pRefrigerator->setAnchorPoint(Vec2(0.5,0.5));
    m_pRefrigerator->setPosition(Vec2(384,568));
    bg->addChild(m_pRefrigerator);

    m_pDoor = Sprite::create("images/ice_pop/3/door_open.png");
    m_pDoor->setAnchorPoint(Vec2(0,0.5));
    m_pDoor->setPosition(Vec2(593,454.5));
    m_pRefrigerator->addChild(m_pDoor,30);

    m_pIce= Sprite::create("images/ice_pop/3/ice.png");
    m_pIce->setAnchorPoint(Vec2(0.5,0.5));
    m_pIce->setPosition(Vec2(299,177));
    m_pRefrigerator->addChild(m_pIce,10);

    m_pRefrigerator->setScale(0.7);
    
    m_pRefrigerator->setVisible(false);
    if(G_IcePop_MouleSprite){
        
        auto ice = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
        ice->setAnchorPoint(Vec2(0.5,0.5));
        ice->setRotation(180);
        ice->setScale(0.85);
        ice->setVisible(false);
        ice->setTag(100);
        ice->setPosition(Vec2(289,466));
        m_pRefrigerator->addChild(ice,6);
        
        auto stick = Sprite::create("images/ice_pop/3/stick.png");
        stick->setAnchorPoint(Vec2(0.5,0.5));
        stick->setScale(0.7);
        stick->setVisible(false);
        stick->setTag(200);
        stick->setPosition(Vec2(289,466 - 100));
        m_pRefrigerator->addChild(stick,4);
        
        
        
        
        
    }
    
     m_pTimeBg = Sprite::create("images/ice_pop/3/count_down/count_bg.png");
    kAdapterScreen->setExactPosition(m_pTimeBg, 320,100 - 1000,Vec2(0.5,1),kBorderNone,kBorderTop);
    this->addChild(m_pTimeBg);

    m_pTimeNum = Sprite::create("images/ice_pop/3/count_down/count_10.png");
    m_pTimeNum->setPosition(Vec2(m_pTimeBg->getContentSize().width/2,m_pTimeBg->getContentSize().height/2));
    m_pTimeNum->setAnchorPoint(Vec2(0.5,0.5));
    m_pTimeBg->addChild(m_pTimeNum);
    
    m_iTimeNum = 10;
    
    return true;
}

void IcePopStepLayer3::onEnter()
{
    StepLayer::onEnter();
    
    if (G_IcePop_MouleSprite) {
        
        m_pMoule = ToolSprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
        kAdapterScreen->setExactPosition(m_pMoule, 10 - 500,10,Vec2(0,1),kBorderLeft,kBorderTop);
        m_pMoule->setDelegate(this);
        m_pMoule->setResponsePoint(Vec2(0,1));
        m_pMoule->addRectByNode(m_pRefrigerator);
        m_pMoule->cancelEvent(true);
        this->addChild(m_pMoule,20);
        
        G_IcePop_MouleSprite->setAnchorPoint(Vec2::ZERO);
        G_IcePop_MouleSprite->setPosition(Vec2::ZERO);
        m_pMoule->addChild(G_IcePop_MouleSprite);
        
        auto ice = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
        ice->setAnchorPoint(Vec2::ZERO);
        ice->setPosition(Vec2::ZERO);
        m_pMoule->addChild(ice,20);
        
        
//
    }
    
    m_pRefrigerator->setScale(0);
    
    m_pRefrigerator->setVisible(true);
    
    
    
    RotateBy* rote = RotateBy::create(1.0, 360*3);
    ScaleTo* scale = ScaleTo::create(1.0, 0.7);
    Spawn* spawn = Spawn::create(rote,scale,NULL);
    m_pRefrigerator->runAction(spawn);
    
    AudioHelp::getInstance()->playEffect("spin magnify.mp3");
    
    auto func = CallFunc::create([&](){
        
        
        handAction1();
        m_pMoule->cancelEvent(false);
    });
    
    ActionUtils::moveBy(m_pMoule, Vec2(500,0),3.0,func);
     cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IcePopStepLayer3::scribbleOver), "ScribbleOver", NULL);
cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IcePopStepLayer3::scribbleTouchBegan), "ScribbleTouchBeagn", NULL);
    
}
void IcePopStepLayer3::handAction1()
{
    Sprite* hand = Sprite::create("images/ice_cream/2/hand.png");
    kAdapterScreen->setExactPosition(hand, 320, 400);
    hand->setAnchorPoint(Vec2(0.5,0.5));
    hand->setTag(102);
    hand->setPosition(Vec2(m_pMoule->getPositionX() + 100,m_pMoule->getPositionY() - 200));
    this->addChild(hand,50);
    
    
    MoveTo* move = MoveTo::create(1.5, Vec2(m_pRefrigerator->getParent()->convertToWorldSpace(m_pRefrigerator->getContentSize()/2)));
    MoveTo* move1 = MoveTo::create(1.5, Vec2(m_pMoule->getPositionX() + 100,m_pMoule->getPositionY() - 200));
    RepeatForever* repeat = RepeatForever::create(Sequence::create(move,move1, NULL));
    
    hand->runAction(repeat);

}
void IcePopStepLayer3::onExit()
{
    StepLayer::onExit();
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "ScribbleOver");
}
void IcePopStepLayer3::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    
}
void IcePopStepLayer3::startTimeAction(float f)
{
    m_iTimeNum--;
    AudioHelp::getInstance()->playEffect("clock.mp3");
    auto func = CallFunc::create([&](){
        
        
        CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
        m_pTimeNum->setScale(1);
        m_pTimeNum->setTexture(__String::createWithFormat("images/ice_pop/3/count_down/count_%d.png",m_iTimeNum)->getCString());
        
        if (m_iTimeNum == 0) {
            this->unschedule(schedule_selector(IcePopStepLayer3::startTimeAction));
            ActionUtils::shakeCup(m_pTimeBg,0,3);
            handAction();
        }
        
    });
    
    m_pTimeNum->runAction(Sequence::create(ScaleTo::create(0.25, 1.3),func,NULL));
    
    
}
void IcePopStepLayer3::scribbleTouchBegan(Ref*)
{
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "ScribbleTouchBeagn");
    if (this->getChildByTag(101)) {
        this->getChildByTag(101)->stopAllActions();
        this->getChildByTag(101)->setVisible(false);
    }
}
void IcePopStepLayer3::handAction()
{
    m_pTimeBg->runAction(ScaleTo::create(0.5, 0));
//    m_pScribbleFreezerDoor->enable();
    Sprite* hand = Sprite::create("images/ice_cream/2/hand.png");
    kAdapterScreen->setExactPosition(hand, 320, 400);
    hand->setAnchorPoint(Vec2(0.5,0.5));
    hand->setTag(101);
    hand->setPosition(Vec2(m_pRefrigerator->getContentSize().width/2,m_pRefrigerator->getContentSize().height/2));
    this->addChild(hand,50);
    
    
    MoveBy* move1 = MoveBy::create(0.5, Vec2(200,0));
    MoveBy* move2 = MoveBy::create(0.5, Vec2(-100,-200));
   
    
    RepeatForever* repeat = RepeatForever::create(Sequence::create(move1,move2,move2->reverse(),move1->reverse(),NULL));
    hand->runAction(repeat);
    
    auto func = CallFunc::create([&](){
        
        
        this->scheduleOnce(schedule_selector(IcePopStepLayer3::getScribbleCallback), 0.2f);
        
    });
    
    ActionUtils::delayTimeAction(this,1.0,func);
    
}
void IcePopStepLayer3::timeAction()
{
      auto func = CallFunc::create([&](){
        
        this->schedule(schedule_selector(IcePopStepLayer3::startTimeAction),0.5f);
          
        frostAction();
        
    });
    ActionUtils::delayTimeAction(this,1.5,func);
    ActionUtils::dropOut(m_pTimeBg, Vec2(0,-1000),1.0);
    
}
void IcePopStepLayer3::frostAction()
{
    
    auto func = CallFunc::create([&](){
        
        
//        this->scheduleOnce(schedule_selector(IcePopStepLayer3::getScribbleCallback), 0.2f);
//        m_pFrost->setVisible(false);
    });
    
    m_pFrost->runAction(Sequence::create(FadeIn::create(6.0),func,NULL));
}
void IcePopStepLayer3::getScribbleCallback(float ft)
{
    m_pScribbleFreezerDoor  = ScribbleTouchNode::create(m_pRefrigerator->getContentSize(),1);
    m_pScribbleFreezerDoor->useTarget("images/ice_pop/3/frost.png");
    m_pScribbleFreezerDoor->useBrush("images/ice_pop/3/frost.png",Scribble::eBrush);
    m_pScribbleFreezerDoor->paint(Vec2::ZERO);
    m_pScribbleFreezerDoor->enable();
//    m_pScribbleFreezerDoor->setPosition(Vec2::ZERO);
//    m_pScribbleFreezerDoor->setAnchorPoint(Vec2::ZERO);
//    m_pRefrigerator->addChild(m_pScribbleFreezerDoor, 35);
    
    m_pScribbleFreezerDoor->setScale(0.7);
    m_pScribbleFreezerDoor->setAnchorPoint(Vec2(0.5,0.5));
    m_pScribbleFreezerDoor->setPosition(Vec2(384,568));
   m_pRefrigerator->getParent()->addChild(m_pScribbleFreezerDoor);
    
    
    m_pScribbleFreezerDoor->useBrush("images/button/scribble.png",Scribble::eEaser);
    m_pFrost->setVisible(false);
    
}

void IcePopStepLayer3::scribbleOver(Ref*)
{
    m_pScribbleFreezerDoor->disable();
    m_pScribbleFreezerDoor->removeFromParent();
    
    //openDoor
    m_pDoor->setTexture("images/ice_pop/3/door_open.png");
    m_pDoor->setAnchorPoint(Vec2(0,0.5));
    m_pDoor->setPosition(Vec2(593,454.5));
    
    //播放粒子效果
    
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/smoke.plist");
    emitter1->setDuration(-1);
    //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    emitter1->setPosition(m_pRefrigerator->getPosition());
    this->addChild(emitter1, 30);
    
    auto func = CallFunc::create([&]{
        
        
        auto particle = ParticleSystemQuad::create("particle/icePop.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(Vec2(400,0));
        this->addChild(particle,20);
        nextStep();
        AudioHelp::getInstance()->playEffect("hurrah.mp3");
    });
    
    ActionUtils::delayTimeAction(this,2.0,func);

}
void IcePopStepLayer3::nextStep()
{
    auto func = CallFunc::create([&](){
        
        //next
        m_pGameLayer->m_bNextAction = true;
        m_pGameLayer->OperateEndToDecoration(IcePopDecorationLayer::create(m_pGameLayer));
    });
    ActionUtils::delayTimeAction(this,3.0,func);
}
void IcePopStepLayer3::closeDoor()
{
    
    m_pDoor->setTexture("images/ice_pop/3/door_close.png");
    m_pDoor->setAnchorPoint(Vec2::ZERO);
    m_pDoor->setPosition(Vec2::ZERO);
    
    //开始倒计时
    m_pFrost = Sprite::create("images/ice_pop/3/frost.png");
    m_pFrost->setAnchorPoint(Vec2::ZERO);
    m_pFrost->setPosition(Vec2::ZERO);
    m_pRefrigerator->addChild(m_pFrost,35);
    m_pFrost->setOpacity(0);
    
    auto func = CallFunc::create([&](){

        
        timeAction();
 
    });
    
    ActionUtils::delayTimeAction(this,0.25,func);

}
void IcePopStepLayer3::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    if (this->getChildByTag(102)) {
        this->getChildByTag(102)->stopAllActions();
        this->getChildByTag(102)->setVisible(false);
    }
}
void IcePopStepLayer3::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
   
}
void IcePopStepLayer3::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    int tag = toolSprite->getTag();
    toolSprite->cancelEvent(true);
    if (m_pRefrigerator->getChildByTag(100)&&m_pRefrigerator->getChildByTag(200)) {
        toolSprite->setVisible(false);
        m_pRefrigerator->getChildByTag(100)->setVisible(true);
        m_pRefrigerator->getChildByTag(200)->setVisible(true);
        G_IcePop_MouleSprite->removeFromParent();
        G_IcePop_MouleSprite->setVisible(true);
        
        G_IcePop_MouleSprite->setAnchorPoint(Vec2(0.5,0.5));
        G_IcePop_MouleSprite->setRotation(180);
        G_IcePop_MouleSprite->setScale(0.85);
        G_IcePop_MouleSprite->setPosition(Vec2(289,466));
        
        m_pRefrigerator->addChild(G_IcePop_MouleSprite,5);

        auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(m_pRefrigerator->getParent()->convertToWorldSpace(m_pRefrigerator->getPosition()));
        this->addChild(particle,20);
        AudioHelp::getInstance()->playEffect("all star .mp3");
        
//        G_IcePop_MouleSprite->release();
        log("3--------%d",G_IcePop_MouleSprite->getReferenceCount());
        
        
        auto func = CallFunc::create([&](){
            
          closeDoor();
            
        });
        
        ActionUtils::delayTimeAction(this,0.5,func);
    }
    
}
void IcePopStepLayer3::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            isReturn = true;
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over with a new Italian Food?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}
void IcePopStepLayer3::onNegativeClick(void* type)
{
    isReturn = false;
}
void IcePopStepLayer3::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  IcePopStepLayer3::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    SceneChangeManager->enterGameScene();
                }
                    break;
            }
        }
    }
    
}
