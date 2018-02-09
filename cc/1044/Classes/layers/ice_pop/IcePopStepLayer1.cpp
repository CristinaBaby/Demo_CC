//
//  IcePopStepLayer1.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/3.
//
//

#include "IcePopStepLayer1.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"


#include "IcePopStepLayer2.h"


IcePopStepLayer1::IcePopStepLayer1(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IcePopStepLayer1::~IcePopStepLayer1()
{
    
}
IcePopStepLayer1* IcePopStepLayer1::create(GameLayer* gameLayer)
{
    IcePopStepLayer1* step1 = new IcePopStepLayer1(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IcePopStepLayer1::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        initData();
        CC_BREAK_IF(!initUI());
        m_bNext = true;
        isInit = true;
        
        m_pCurrentSprite = nullptr;
        m_iTouchIndex = -1;
    } while (0);
    
    return isInit;
}
void IcePopStepLayer1::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<6; i++) {
        
        __String* str = __String::createWithFormat("images/ice_pop/1/mould_%d_1.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
        
        __String* str1 = __String::createWithFormat("images/ice_pop/1/mould_%d.png",i);
        Sprite* mould = Sprite::create(str1->getCString());
        mould->setPosition(Vec2::ZERO);
        mould->setAnchorPoint(Vec2::ZERO);
        sprite->addChild(mould);
        
//        __String* strData = __String::create("mould");
//        
//        sprite->setUserData((char*)strData->getCString());
        
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool IcePopStepLayer1::initUI()
{
    Sprite* bg = Sprite::create("images/ice_pop/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    Sprite* bg1 = Sprite::create("images/ice_pop/1/bg1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1);
    
    m_pIce= ToolSprite::create("images/ice_pop/1/ice.png");
    m_pIce->setDelegate(this);
    m_pIce->setIsMove(false);
    kAdapterScreen->setExactPosition(m_pIce, 320, 200,Vec2(0.5,0.5),kBorderNone,kBorderTop);
    this->addChild(m_pIce);
 
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 700 + 30));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    //    m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 30;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
//    m_pGirdView->setScrollEnable(true);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0 - 50 - 30,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,7);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(6);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IcePopStepLayer1::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
     m_pGirdView->m_pScrollView->setContentOffset(Vec2(-2500, 0));
    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    return true;
}
void IcePopStepLayer1::onEnter()
{
    StepLayer::onEnter();
    m_pGirdView->refresh();

//    m_pGirdView->setScrollEnable(true);
    m_pGirdView->setEnable(true);
    m_bNext = true;
}
void IcePopStepLayer1::onExit()
{
    StepLayer::onExit();
}
void IcePopStepLayer1::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    chooseAction();
    
    RotateBy* rote = RotateBy::create(0.5, 30);
    RepeatForever* repeat = RepeatForever::create(rote);
    m_pIce->runAction(repeat);
}
void IcePopStepLayer1::titleAction()
{
    //     ActionUtils::dropOut(m_pTitle, Vec2(0,-400),2.0);
    
    
    auto func = CallFunc::create([&](){
        
        
//        m_pGirdView->setScrollEnable(true);
        
    });
    
    ActionUtils::delayTimeAction(this,2.0,func);
    
}
void IcePopStepLayer1::chooseAction()
{
   
    
    auto func = CallFunc::create([&](){
        
        titleAction();
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 2);
    });
    ActionUtils::delayTimeAction(this,0.5,func);
    
}

//滑动杯子点击回调
void IcePopStepLayer1::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
    if (kIAPManager->isFree("mould", index)) {
//        m_pGirdView->setScrollEnable(false);
        int y = item->getPositionY();
        
        G_IcePop_MouleNum = index + 1;
        
        //出现下一步
        if (m_iTouchIndex == index) {
            return;
        }else if(m_pCurrentSprite){
            
            m_pCurrentSprite->stopAllActions();
            m_pCurrentSprite->setPositionY(y);
        }
        
        
        m_pCurrentSprite = item;
        m_iTouchIndex = index;
        
        
        auto func = CallFunc::create([=](){
            
            
            
            m_pGirdView->setEnable(true);
            
        });
        
        
       
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        item->addChild(chooseParticle,4);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        ActionUtils::jumpAction(item, 2);
        
        ActionUtils::delayTimeAction(this,1.0,func);
        
        CCLOG("%d",index);

        if (!m_pGameLayer->m_bNextButtonIsShow) {
            m_bNext = true;
        }
        if (m_bNext) {
            m_bNext = false;
            m_pGameLayer->m_bNextAction = false;
            m_pGameLayer->doNextStep(IcePopStepLayer2::create(m_pGameLayer));
        }
        
        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }

    
    
}
void IcePopStepLayer1::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    m_pIce->cancelEvent(true);
    
    
    auto func = CallFunc::create([&](){
        
        m_pIce->cancelEvent(false);
    
    });
    
    ScaleTo* scale = ScaleTo::create(0.25, 1.3);
    ScaleTo* scale1 = ScaleTo::create(0.25, 1);
    toolSprite->runAction(Sequence::create(scale,scale1,func, NULL));
    

}
void IcePopStepLayer1::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void IcePopStepLayer1::onNegativeClick(void* type)
{
    isReturn = false;
}
void IcePopStepLayer1::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  IcePopStepLayer1::touchEvent(Ref *obj , Widget::TouchEventType type)
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
