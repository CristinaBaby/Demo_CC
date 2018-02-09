//
//  IceCreamIceCreamStepLayer1.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/2.
//
//

#include "IceCreamStepLayer1.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"

#include "IceCreamStepLayer2.h"


IceCreamStepLayer1::IceCreamStepLayer1(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IceCreamStepLayer1::~IceCreamStepLayer1()
{
    log("~IceCreamStepLayer1");
}
IceCreamStepLayer1* IceCreamStepLayer1::create(GameLayer* gameLayer)
{
    IceCreamStepLayer1* step1 = new IceCreamStepLayer1(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IceCreamStepLayer1::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
          initData();
        
        CC_BREAK_IF(!initUI());
        m_iTouchIndex = -1;
        
        m_bNext = true;
        
        isInit = true;
        
        m_iTouchIndex = -1;
        
        m_pCurrentSprite = nullptr;
        
    } while (0);
    
    return isInit;
    
    
    
}
void IceCreamStepLayer1::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<13; i++) {

        __String* str = __String::createWithFormat("images/ice_cream/1/cup_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
//        __String* strData = __String::create("icecup");
//        
//        strData->retain();
//        sprite->setUserData((char*)strData->getCString());
        
        //加载盖子
        
        __String* str1 = __String::createWithFormat("images/ice_cream/1/cup_%d_1.png",i);
        Sprite* cup = Sprite::create(str1->getCString());
        cup->setPosition(Vec2::ZERO);
        cup->setAnchorPoint(Vec2::ZERO);
        sprite->addChild(cup);
       
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool IceCreamStepLayer1::initUI()
{
    Sprite* bg = Sprite::create("images/ice_cream/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 700));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    //    m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 40;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0 - 50 - 50,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,7);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(3);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IceCreamStepLayer1::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;

    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-2500, 0));
    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    return true;
}
void IceCreamStepLayer1::onEnter()
{
    StepLayer::onEnter();
    
    if(m_pGirdView){
    m_pGirdView->refresh();
    }
    
    m_pCurrentSprite = nullptr;
//    m_pGirdView->setScrollEnable(true);
    m_pGirdView->setEnable(true);
    m_bNext = true;
}
void IceCreamStepLayer1::onExit()
{
    StepLayer::onExit();
}
void IceCreamStepLayer1::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
   chooseAction();
}
void IceCreamStepLayer1::titleAction()
{
   auto func = CallFunc::create([&](){
        
        
//        m_pGirdView->setScrollEnable(true);
       m_pGirdView->setEnable(true);
    });
    
    ActionUtils::delayTimeAction(this,2.0,func);
    
}
void IceCreamStepLayer1::chooseAction()
{
    
    
    auto func = CallFunc::create([&](){
        
        titleAction();
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 2);
    });
     ActionUtils::delayTimeAction(this,0.5,func);
}

//滑动杯子点击回调
void IceCreamStepLayer1::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
    if (kIAPManager->isFree("icecup", index)) {
        m_pGirdView->setEnable(true);
        int y = item->getPositionY();
        
        G_IceCream_CupNum = index + 1;
        //出现下一步
        if (m_iTouchIndex == index) {
            return;
        }else if(m_pCurrentSprite){
            
            m_pCurrentSprite->stopAllActions();
            m_pCurrentSprite->setPositionY(y);
        }
        
        m_pCurrentSprite = item;
        m_iTouchIndex = index;
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        item->addChild(chooseParticle,4);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        auto func = CallFunc::create([&](){
            
           
            
        });
        if(m_bNext){
            m_bNext = false;
            m_pGameLayer->m_bNextAction = false;
            m_pGameLayer->doNextStep(IceCreamStepLayer2::create(m_pGameLayer));
        }
        //    ActionUtils::shakeCup(item, 0.5, 3);
        ActionUtils::jumpAction(item, 2);
        
        ActionUtils::delayTimeAction(this,1.0,func);
        
        CCLOG("%d",G_IceCream_CupNum);
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    
    }
    
    
    
}
void IceCreamStepLayer1::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void IceCreamStepLayer1::onNegativeClick(void* type)
{
    isReturn = false;
}
void IceCreamStepLayer1::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  IceCreamStepLayer1::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    SceneChangeManager->enterGameScene();                }
                    break;
            }
        }
    }
    
}











