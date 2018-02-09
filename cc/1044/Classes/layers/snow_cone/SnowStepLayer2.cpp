//
//  SnowStepLayer2.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#include "SnowStepLayer2.h"
#include "SnowStepLayer3.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"



SnowStepLayer2::SnowStepLayer2(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
SnowStepLayer2::~SnowStepLayer2()
{
    
}
SnowStepLayer2* SnowStepLayer2::create(GameLayer* gameLayer)
{
    SnowStepLayer2* step1 = new SnowStepLayer2(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool SnowStepLayer2::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
        initData();
        
        CC_BREAK_IF(!initUI());
        
        m_bNext = true;
        
        isInit = true;
        
    } while (0);
    
    return isInit;
    
    
    
}
void SnowStepLayer2::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<20; i++) {

        __String* str = __String::createWithFormat("images/snow_cone/2/cup/icon/cup%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
        sprite->setScale(0.5);
        m_vTypes.pushBack(sprite);
        
        
//        __String* strData = __String::create("cup");
//        strData->retain();
//        
//        sprite->setUserData((char*)strData->getCString());
        

        
    }
    
}
bool SnowStepLayer2::initUI()
{
    Sprite* bg = Sprite::create("images/snow_cone/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    m_pGirdViewBg  = Utils::getUISprite("images/dec/dec_bar.png");
    kAdapterScreen->setExactPosition(m_pGirdViewBg,320 ,20,Vec2(0.5, 1),kBorderNone,kBorderTop);
    this->addToUILayer(m_pGirdViewBg, 5);
    
    
    m_pGirdView = KDGridView::create(Size(607, 180 + 30));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    //    m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 0;
    m_pGirdView->m_fMarginLeft = 0;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    m_pGirdView->setAnchorPoint(Vec2(0.5,0.5));
    m_pGirdView->setPosition(Vec2(m_pGirdViewBg->getContentSize().width/2,m_pGirdViewBg->getContentSize().height/2 + 50));
    m_pGirdViewBg->addChild(m_pGirdView,20);

    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(0);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SnowStepLayer2::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-1500 , 0));
    
    m_pCup = Sprite::create("images/snow_cone/2/cup/cup1.png");
    kAdapterScreen->setExactPosition(m_pCup, 320, 200,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(m_pCup);
    m_pPos = m_pCup->getPosition();
    m_pCup->setScale(0);
    
    m_pGirdViewBg->setPositionX(m_pGirdViewBg->getPositionX()-1000);
    
    titleAction();
    
    auto listener = _eventDispatcher->addCustomEventListener("after_purchase",CC_CALLBACK_1(SnowStepLayer2::afterPurchase, this));
    
    
    return true;
}

void SnowStepLayer2::afterPurchase(EventCustom* e)
{
    m_pGirdView->refresh();
}

void SnowStepLayer2::onEnter()
{
    StepLayer::onEnter();
    m_pAdapter->setData(m_vTypes);
    m_pGirdView->refresh();

//    m_pGirdView->setScrollEnable(true);
    m_pGirdView->setEnable(true);
    m_bNext = true;
    
}
void SnowStepLayer2::onExit()
{
    StepLayer::onExit();
}
void SnowStepLayer2::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
}
void SnowStepLayer2::titleAction()
{
    auto func = CallFunc::create([&](){
        
        chooseAction();
       
    });
    
   ActionUtils::moveByEaseBack(m_pGirdViewBg, Vec2(1000,0),1.0,func);
    
}
void SnowStepLayer2::chooseAction()
{

//    m_pGirdView->setScrollEnable(true);
    m_pGirdView->setEnable(true);
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 2);

}

//滑动杯子点击回调
void SnowStepLayer2::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    if (kIAPManager->isFree("cup", index)) {
        
//        m_pGirdView->setScrollEnable(false);
        
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
        m_pCurrentSprite = nullptr;
        
        G_Snow_CupNum = index;
        log("%d",G_Snow_CupNum);
        __String* str = __String::createWithFormat("images/snow_cone/2/cup/cup%d.png",index);
        m_pCup->setTexture(str->getCString());
        
        if (m_bNext) {
            m_bNext = false;
            m_pGameLayer->m_bNextAction = false;
            m_pGameLayer->doNextStep(SnowStepLayer3::create(m_pGameLayer));
        }
        if (!m_pGameLayer->m_bNextButtonIsShow) {
            m_bNext = true;
        }
        
        m_pCup->setScale(0);
        m_pCup->setPosition(item->getParent()->convertToWorldSpace(item->getPosition()));
        
        ScaleTo* scale = ScaleTo::create(0.5, 0.7);
        MoveTo* move = MoveTo::create(0.5,m_pPos);
        Spawn* sp = Spawn::create(scale,move, NULL);
        
        auto func = CallFunc::create([&](){
            
//            m_pGirdView->setScrollEnable(true);
            m_pGirdView->setEnable(true);
            
            auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
            particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            particle->setVisible(true);
            particle->setPosition(m_pCup->getParent()->convertToWorldSpace(m_pCup->getPosition() + Point(0,200)));
            this->addChild(particle,20);
            AudioHelp::getInstance()->playEffect("all star .mp3");
            
        });
        m_pCup->runAction(Sequence::create(sp,func, NULL));

        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }
    
    
    
    
}
void SnowStepLayer2::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void SnowStepLayer2::onNegativeClick(void* type)
{
    isReturn = false;
}
void SnowStepLayer2::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  SnowStepLayer2::touchEvent(Ref *obj , Widget::TouchEventType type)
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

