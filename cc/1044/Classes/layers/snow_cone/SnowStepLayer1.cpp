//
//  SnowStepLayer1.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/7.
//
//

#include "SnowStepLayer1.h"
#include "SnowStepLayer2.h"
#include "SnowStepLayer5.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"



SnowStepLayer1::SnowStepLayer1(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
SnowStepLayer1::~SnowStepLayer1()
{
    
}
SnowStepLayer1* SnowStepLayer1::create(GameLayer* gameLayer)
{
    SnowStepLayer1* step1 = new SnowStepLayer1(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool SnowStepLayer1::init()
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
void SnowStepLayer1::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<13; i++) {
        
        __String* str = __String::createWithFormat("images/ice_cream/1/cup_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        __String* strData = __String::create("cup");
//        sprite->setUserData((char*)strData->getCString());

        //加载盖子
        if (i!=11) {
            __String* str1 = __String::createWithFormat("images/ice_cream/1/cup_%d_1.png",i);
            Sprite* cup = Sprite::create(str1->getCString());
            cup->setPosition(Vec2::ZERO);
            cup->setAnchorPoint(Vec2::ZERO);
            sprite->addChild(cup);
        }
        
        
        
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool SnowStepLayer1::initUI()
{
    Sprite* bg = Sprite::create("images/snow_cone/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    m_pMachine = Sprite::create("images/snow_cone/1/icemachion1.png");
    //    machine_ -> setPosition(KSVisibleRect::getPosition(303, 331));
    kAdapterScreen->setExactPosition(m_pMachine,  303, 371);
    this -> addChild(m_pMachine,2);
    
    Sprite* box = Sprite::create("images/snow_cone/1/box1.png");
    box->setPosition(Vec2::ZERO);
    box->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(box,1);
    
    m_pBox = Sprite::create("images/snow_cone/1/box2.png");
    m_pBox->setPosition(Vec2::ZERO);
    m_pBox->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(m_pBox,8);
    
    Sprite* machine = Sprite::create("images/snow_cone/1/icemachion2.png");
    machine->setPosition(Vec2::ZERO);
    machine->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(machine,10);
    
    m_pMachineCover  = ToolSprite::create("images/snow_cone/1/snowcone_cover.png");
    m_pMachineCover -> setAnchorPoint(Vec2(0.05, 0.285));
    m_pMachineCover->setPosition(Vec2(178,520));
    m_pMachineCover->setDelegate(this);
    m_pMachineCover->setTag(KMachineCoverUp);
//    m_pMachineCover->cancelEvent(true);
    m_pMachineCover->setIsMove(false);
    m_pMachine->addChild(m_pMachineCover,15);

    m_pSwitchBtn = ToolSprite::create("images/snow_cone/1/btn_button_h.png");
    m_pSwitchBtn -> setAnchorPoint(Vec2(0.5, 0.5));
    m_pSwitchBtn->setPosition(Vec2(222,373));
    m_pSwitchBtn->setDelegate(this);
    m_pSwitchBtn->setTag(KSwitchBtn);
    m_pSwitchBtn->cancelEvent(true);
    m_pSwitchBtn->setIsMove(false);
    m_pMachine->addChild(m_pSwitchBtn,20);
    
    
    m_pHint = Sprite::create("images/snow_cone/1/btn_gyrate.png");
    //    hint_ -> setPosition(KSVisibleRect::getPosition(301, 709));
    kAdapterScreen->setExactPosition(m_pHint,303, 331 + 300);
    this -> addChild(m_pHint,20);
    m_pHint->setVisible(false);
   

    m_pIceBag = ToolSprite::create("images/snow_cone/1/ice_pack.png");
    kAdapterScreen->setExactPosition(m_pIceBag,482+700, 709 + 100);
    m_pIceBag -> setRotation(67.5);
    m_pIceBag->setDelegate(this);
    m_pIceBag->cancelEvent(true);
    m_pIceBag->setTag(KIceBag);
    m_pIceBag->setIsMove(false);
    this -> addChild(m_pIceBag,3);
    
    
    
    m_pMachine->setPositionX(m_pMachine->getPositionX()+1000);
    
    
    
    
    return true;
}
void SnowStepLayer1::onEnter()
{
    StepLayer::onEnter();
    
    titleAction();
    
}
void SnowStepLayer1::onExit()
{
    StepLayer::onExit();
}
void SnowStepLayer1::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
  
}
void SnowStepLayer1::titleAction()
{
   
    auto func = CallFunc::create([&](){
        ActionUtils::flashAction(m_pHint, -1);
    });
    ActionUtils::moveByEaseBack(m_pMachine, Vec2(-1000,0),1.0,func);
    
}
void SnowStepLayer1::showIceBag()
{
    m_pIceBag->cancelEvent(false);
    auto func = CallFunc::create([&](){
        
//        m_pHint->setVisible(true);
        ActionUtils::zoomAction(m_pIceBag, -1);
        
    });
    MoveBy* move = MoveBy::create(1,Vec2(-700,0));
    m_pIceBag->runAction(Sequence::create(move,func, NULL));
    

   
}
void SnowStepLayer1::startMakeIce()
{
    AudioHelp::getInstance()->playEffect("scoop up the ice.mp3");
    Sprite* ice = Sprite::create("images/snow_cone/1/ice4.png");
    ice->setPosition(Vec2(226,249));
    m_pMachine->addChild(ice,6);
    ActionUtils::flashAction(ice, 4);
    
    m_pIce = Sprite::create("images/snow_cone/1/ice2.png");
    m_pIce->setPosition(Vec2::ZERO);
    m_pIce->setAnchorPoint(Vec2::ZERO);
    m_pMachine->addChild(m_pIce,5);
    m_pIce->setVisible(false);
    
    AudioHelp::getInstance()->playEffect("grind the ice.mp3");
    
    auto func = CallFunc::create([&](){
        m_pIce->setVisible(true);
    });
    ActionUtils::delayTimeAction(this,1.0,func);
    auto func1 = CallFunc::create([&](){
        
        
        
        m_pIce->setTexture("images/snow_cone/1/ice3.png");
        m_pSwitchBtn->setTexture("images/snow_cone/1/btn_button_h.png");
        
        auto particle = ParticleSystemQuad::create("particle/snowCone.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(Vec2(400,0));
        this->addChild(particle,4);
        
        AudioHelp::getInstance()->playEffect("hurrah.mp3");

        nextStep();
    });

    

    
    ActionUtils::delayTimeAction(this,3.0,func1);
}
void SnowStepLayer1::nextStep()
{
    auto func = CallFunc::create([&](){

        nextStep1();
    });
    ActionUtils::delayTimeAction(this,3.0,func);

}
void SnowStepLayer1::nextStep1()
{
    m_pGameLayer->showNextButton(true);
    m_pGameLayer->m_bNextAction = true;
    m_pGameLayer->doNextStep(SnowStepLayer2::create(m_pGameLayer));
}
void SnowStepLayer1::showHand()
{
    m_pSwitchBtn->cancelEvent(false);
    m_pHand =Sprite::create("images/snow_cone/1/hand.png");
    m_pHand->setAnchorPoint(Vec2(0.5,1));
    m_pHand->setPosition(Vec2(222,373));
    m_pMachine->addChild(m_pHand,30);
    ActionUtils::zoomAction(m_pHand,-1);
}
void SnowStepLayer1::iceBagClicked()
{
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
    emitter1->setPosition(Vec2(m_pIceBag->getParent()->convertToWorldSpace(m_pIceBag->getPosition())));
    emitter1->setDuration(0.5f);
    emitter1->setAutoRemoveOnFinish(true);
    this->addToContentLayer(emitter1, 10);
    AudioHelp::getInstance()->playEffect("all star .mp3");
    auto func = CallFunc::create([&](){
        
        iceBagAction();
    });
    
    Sprite* ice = Sprite::create("images/snow_cone/1/ice1.png");
    ice->setPosition(Vec2(m_pMachine->getContentSize().width - 150, m_pMachine->getContentSize().height + 150));
    m_pMachine->addChild(ice,5);

    auto move = MoveBy::create(0.5, Vec2(0, -300));
    
    
    ice -> runAction(Sequence::create(move,
                                      Hide::create()
                                      ,func, NULL));
    AudioHelp::getInstance()->playEffect("pour ice.mp3");
//
    
}
void SnowStepLayer1::iceBagAction()
{
    
    JumpBy* jump = JumpBy::create(0.5, Vec2(0,0), 30, 1);
    
    auto func = CallFunc::create([&](){
        
        iceAction();
        
    });
    
    m_pIceBag->runAction(Sequence::create(jump,func, NULL));
    AudioHelp::getInstance()->playEffect("pour ice.mp3");
}
 void SnowStepLayer1::iceAction()
{
    
    auto func = CallFunc::create([&](){
        
        m_pMachineCover->cancelEvent(false);
        m_pHint->setVisible(true);
        m_pHint->setTexture("images/snow_cone/1/btn_rightgyrate.png");
        ActionUtils::flashAction(m_pHint, -1);
        
        m_pIceBag->runAction(MoveBy::create(1, Vec2(700,0)));

        
    });
    
    Sprite* ice = Sprite::create("images/snow_cone/1/ice1.png");
    ice->setPosition(Vec2(m_pMachine->getContentSize().width - 150, m_pMachine->getContentSize().height + 150));
    m_pMachine->addChild(ice,5);
    
    auto move = MoveBy::create(0.5, Vec2(0, -300));
    
    ice -> runAction(Sequence::create(move,
                                      Hide::create(),func,NULL));

}
//滑动杯子点击回调
void SnowStepLayer1::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if (kIAPManager->isFree("cup", index)) {
        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }

    CCLOG("%d",index);
    
}
void SnowStepLayer1::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    switch (tag) {
        case KMachineCoverUp:
            {
                log("KMachineCoverUp  %f",pTouch->getDelta().x);
            }
            
            
            break;
        case KSwitchBtn:
            {
                //按钮转换形态
                toolSprite->cancelEvent(true);
                m_pHand->removeFromParent();
                toolSprite->setTexture("images/snow_cone/1/btn_button.png");

                startMakeIce();
            }
            break;
        case KIceBag:
            {
                toolSprite->cancelEvent(true);
                auto func = CallFunc::create([&](){
                    
                    //点击冰袋
                    iceBagClicked();
                    
                });
                
                RotateBy* rote = RotateBy::create(1, -80);
                toolSprite->runAction(Sequence::create(rote,func, NULL));
            }
            break;
        default:
            break;
    }
    
}
void SnowStepLayer1::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    log("KMachineCoverMove");
    int tag = toolSprite->getTag();
    if (tag == KMachineCoverUp && pTouch->getDelta().x<=0) {
        
        m_pHint->stopAllActions();
        m_pHint->setVisible(false);
        
        AudioHelp::getInstance()->playEffect("7.lidcome.mp3");
        
        //转动开关
        m_pMachineCover->setTag(KMachineCoverDown);
        m_pMachineCover->cancelEvent(true);
        
        auto func = CallFunc::create([&](){
            
            showIceBag();
            
        });
        
        RotateBy* rote = RotateBy::create(1.0, -180);
        m_pMachineCover->runAction(Sequence::create(rote,func, NULL));
        
        
    }else if (tag == KMachineCoverDown && pTouch->getDelta().x>=0){
        
        AudioHelp::getInstance()->playEffect("7.lidcome.mp3");
        m_pHint->stopAllActions();
        m_pHint->setVisible(false);
        
        
       
        
        m_pMachineCover->cancelEvent(true);
        
        auto func = CallFunc::create([&](){
            
            showHand();
            
        });
        
        
        RotateBy* rote = RotateBy::create(1.0, 180);
        m_pMachineCover->runAction(Sequence::create(rote,func, NULL));
        
        
    }


}
void SnowStepLayer1::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    
}
void SnowStepLayer1::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
    
}

void SnowStepLayer1::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void SnowStepLayer1::onNegativeClick(void* type)
{
    isReturn = false;
}
void SnowStepLayer1::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  SnowStepLayer1::touchEvent(Ref *obj , Widget::TouchEventType type)
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
