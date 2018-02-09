//
//  IceCreamStepLayer2.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/2.
//
//

#include "IceCreamStepLayer2.h"
#include "IceCreamStepLayer3.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"



IceCreamStepLayer2::IceCreamStepLayer2(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IceCreamStepLayer2::~IceCreamStepLayer2()
{
    
}
IceCreamStepLayer2* IceCreamStepLayer2::create(GameLayer* gameLayer)
{
    IceCreamStepLayer2* step1 = new IceCreamStepLayer2(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IceCreamStepLayer2::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
        initData();
        m_bIsFirst = true;
        
        
        CC_BREAK_IF(!initUI());
        
        
        
        isInit = true;
        
    } while (0);
    
    return isInit;
    
    
    
}
//初始化冰淇淋数据
void IceCreamStepLayer2::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<15; i++) {

        __String* str = __String::createWithFormat("images/ice_cream/2/Ice cream_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
//        __String* strData = __String::create("icecream");
//        
//        sprite->setUserData((char*)strData->getCString());
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool IceCreamStepLayer2::initUI()
{
    Sprite* bg = Sprite::create("images/ice_cream/2/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    Sprite* bg1 = Sprite::create("images/ice_cream/2/bg_1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1);
    
//    m_pFreezerDoorTop = Sprite::create("images/ice_cream/2/Freezer door_1.png");
//    kAdapterScreen->setExactPosition(m_pFreezerDoorTop, 0, 160,Vec2(0,0),kBorderNone,kBorderBottom);
//    this->addChild(m_pFreezerDoorTop,20);
//    m_pFreezerDoorTop->setVisible(false);

    
    m_pFreezerDoorBottom = Sprite::create("images/ice_cream/2/Freezer door_5.png");
    kAdapterScreen->setExactPosition(m_pFreezerDoorBottom, 0, 160,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pFreezerDoorBottom,15);
    
    
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 700),1);
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
        m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    m_pGirdView->m_pScrollView->setTouchEnabled(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0-215,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,5);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(4);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IceCreamStepLayer2::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    

    for(int i = 1;i<4;i++)
    {
        Sprite* frezer = Sprite::create(__String::createWithFormat("images/ice_cream/2/Freezer door_%d.png",i)->getCString());
        kAdapterScreen->setExactPosition(frezer, 0, 160,Vec2(0,0),kBorderLeft,kBorderBottom);
        //设置TAG
        frezer->setTag(i + 100);
        this->addChild(frezer,20 - i);
    }
    
    return true;
}
void IceCreamStepLayer2::onEnter()
{
    StepLayer::onEnter();

    if (m_bIsFirst) {
        m_bIsFirst=false;
        this->scheduleOnce(schedule_selector(IceCreamStepLayer2::getScribbleCallback), 0.2f);
    }
    
//    m_pGirdView->refresh();
    if(m_pGirdView){
        m_pGirdView->refresh();
    }
//    m_pGirdView->setEnable(true);
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IceCreamStepLayer2::scribbleOver), "ScribbleOver", NULL);
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IceCreamStepLayer2::scribbleTouchBegan), "ScribbleTouchBeagn", NULL);
   
}
void IceCreamStepLayer2::onExit()
{
    StepLayer::onExit();
     cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "ScribbleOver");
    
    
}
void IceCreamStepLayer2::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    chooseAction();
}
void IceCreamStepLayer2::scribbleTouchBegan(Ref*)
{
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "ScribbleTouchBeagn");
    if (this->getChildByTag(101)) {
        this->getChildByTag(101)->stopAllActions();
        this->getChildByTag(101)->setVisible(false);
    }
}
//擦拭完成的回调
void IceCreamStepLayer2::scribbleOver(Ref*)
{
    log("---ScribbleOver");
    m_pScribbleFreezerDoor->disable();
    m_pScribbleFreezerDoor->removeFromParent();
//    m_pGirdView->setScrollEnable(true);
    m_pGirdView->setEnable(false);
    
    auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
    particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    particle->setVisible(true);
    particle->setPosition(m_pFreezerDoorBottom->getParent()->convertToWorldSpace(m_pFreezerDoorBottom->getPosition()) + Point(m_pFreezerDoorBottom->getContentSize()/2));
    this->addChild(particle,20);
    AudioHelp::getInstance()->playEffect("all star .mp3");
    
    auto func = CallFunc::create([&](){
//        m_pGirdView->setScrollEnable(true);
        m_pGirdView->m_pScrollView->setTouchEnabled(true);
        
        auto particle = ParticleSystemQuad::create("particle/iceCream.plist");
        particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
        particle->setVisible(true);
        particle->setPosition(Vec2(400,0));
        this->addChild(particle,20);
        
        AudioHelp::getInstance()->playEffect("hurrah.mp3");
        
        nextStep();
    });
    
    
    auto pOvenAnimation = Animation::create();
    pOvenAnimation->addSpriteFrameWithFile("images/ice_cream/2/Freezer door_5.png");
    pOvenAnimation->addSpriteFrameWithFile("images/ice_cream/2/Freezer door_6.png");
    pOvenAnimation->addSpriteFrameWithFile("images/ice_cream/2/Freezer door_7.png");
    pOvenAnimation->setDelayPerUnit(0.5f);
    pOvenAnimation->setLoops(1);
    
    auto pOvenAnimat = Animate::create(pOvenAnimation);

    m_pFreezerDoorBottom->runAction(Sequence::create(DelayTime::create(1.0),pOvenAnimat,func,NULL));
    
    
}
void IceCreamStepLayer2::nextStep()
{
    auto func = CallFunc::create([&](){
        
        //next
        m_pGameLayer->m_bNextAction = true;
        m_pGameLayer->doNextStep(IceCreamStepLayer3::create(m_pGameLayer));
    });
    ActionUtils::delayTimeAction(this,3.0,func);
}
//冰柜门的动画渐变
void IceCreamStepLayer2::freezerAction()
{
    
    //渐变完成之后，改变第一个冰柜的层级，在其上增加一个擦拭层
    auto func = CallFunc::create([&](){
        //手势动画
        handAction();

    });

    //玻璃门渐变
    FadeOut* fade = FadeOut::create(1.0);
    auto fade1 = fade->clone();
    auto fade2 = fade->clone();
   
    if(this->getChildByTag(101)&&this->getChildByTag(102)&&this->getChildByTag(103)){
        this->getChildByTag(101)->runAction(Sequence::create(fade,RemoveSelf::create(), NULL));
        this->getChildByTag(102)->runAction(Sequence::create(DelayTime::create(1.0),fade1,RemoveSelf::create(), NULL));
        this->getChildByTag(103)->runAction(Sequence::create(DelayTime::create(2.0),fade2,RemoveSelf::create(), func,NULL));
    }
//
    
}
void IceCreamStepLayer2::handAction()
{
    
    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/smoke.plist");
    emitter1->setDuration(-1);
    //    emitter1->setPosition(sprite->getParent()->convertToWorldSpace(sprite->getPosition()));
    emitter1->setAutoRemoveOnFinish(true);
    emitter1->setPosition(m_pFreezerDoorBottom->getParent()->convertToWorldSpace(m_pFreezerDoorBottom->getPosition()) + Point(m_pFreezerDoorBottom->getContentSize()/2));
    this->addChild(emitter1, 30);
    
//    ParticleSystemQuad *emitter2 = ParticleSystemQuad::create("particle/snow.plist");
//    emitter2->setDuration(-1);
//    emitter2->setPosition(m_pFreezerDoorBottom->getParent()->convertToWorldSpace(m_pFreezerDoorBottom->getPosition()) + Point(m_pFreezerDoorBottom->getContentSize()/2));
//    emitter2->setAutoRemoveOnFinish(true);
//    this->addChild(emitter2, 30);
    
    m_pScribbleFreezerDoor->enable();
    
    Sprite* hand = Sprite::create("images/ice_cream/2/hand.png");
    kAdapterScreen->setExactPosition(hand, 320, 400);
    hand->setTag(101);
    this->addChild(hand,20);
   
   
    MoveBy* move1 = MoveBy::create(0.5, Vec2(200,0));
    MoveBy* move2 = MoveBy::create(0.5, Vec2(-100,-200));
    
    
    RepeatForever* repeat = RepeatForever::create(Sequence::create(move1,move2,move2->reverse(),move1->reverse(),NULL));
    hand->runAction(repeat);
    
}
void IceCreamStepLayer2::chooseAction()
{
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-5200, 0));
    
    auto func = CallFunc::create([&](){
        
        freezerAction();
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(20, 0), 2);
        
        
        
    });
    ActionUtils::delayTimeAction(this,0.5,func);
   
}


void IceCreamStepLayer2::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
//    m_pGirdView->setEnable(false);
//    if (kIAPManager->isFree("icecream", index)) {
//        m_pGirdView->setEnable(true);
//    }else{
//    
//       SceneManager::getInstance()->enterShopScene();
//    }
    
//    m_pCurrentSprite = nullptr;
//    
//    int n = (uintptr_t)item->getUserData();
//    CCLOG("%d",index);
    
}
void IceCreamStepLayer2::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void IceCreamStepLayer2::onNegativeClick(void* type)
{
    isReturn = false;
}
void IceCreamStepLayer2::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void IceCreamStepLayer2::getScribbleCallback(float ft)
{
    m_pScribbleFreezerDoor  = ScribbleTouchNode::create(Size(visibleSize.width, visibleSize.height),1);
    m_pScribbleFreezerDoor->setZOrder(100);
    m_pScribbleFreezerDoor->useTarget("images/ice_cream/2/Freezer door_4.png");
    m_pScribbleFreezerDoor->useBrush("images/ice_cream/2/Freezer door_1.png",Scribble::eBrush);
    m_pScribbleFreezerDoor->paint(Vec2::ZERO);
    kAdapterScreen->setExactPosition(m_pScribbleFreezerDoor, 0 + 320, 160 + 220,Vec2(0,0),kBorderNone,kBorderBottom);
    this->addChild(m_pScribbleFreezerDoor, 16);
    m_pScribbleFreezerDoor->disable();
    m_pScribbleFreezerDoor->useBrush("images/button/scribble.png",Scribble::eEaser);
  
}
void  IceCreamStepLayer2::touchEvent(Ref *obj , Widget::TouchEventType type)
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

