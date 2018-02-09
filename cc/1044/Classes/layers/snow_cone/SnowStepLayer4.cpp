//
//  SnowStepLayer4.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#include "SnowStepLayer4.h"
#include "SnowStepLayer5.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"



SnowStepLayer4::SnowStepLayer4(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
SnowStepLayer4::~SnowStepLayer4()
{
    
}
SnowStepLayer4* SnowStepLayer4::create(GameLayer* gameLayer)
{
    SnowStepLayer4* step1 = new SnowStepLayer4(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool SnowStepLayer4::init()
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
void SnowStepLayer4::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<9; i++) {

        __String* str = __String::createWithFormat("images/snow_cone/4/icon/shape_%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
//        __String* strData = __String::create("shape");
//        sprite->setUserData((char*)strData->getCString());
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool SnowStepLayer4::initUI()
{
    Sprite* bg = Sprite::create("images/snow_cone/4/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    Sprite* bg1 = Sprite::create("images/snow_cone/4/bg1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1,20);

    m_pGridViewBg = Sprite::create("images/ice_cream/3/popup_bg.png");
    kAdapterScreen->setExactPosition(m_pGridViewBg, 320,480);
    this->addChild(m_pGridViewBg,10);
    
    m_pGirdView = KDGridView::create(Size(505, 250));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
  
    m_pGirdView->m_fHSpace = 30;
    m_pGirdView->m_fMarginTop = 5;

    m_pGirdView->setAnchorPoint(Vec2(0.5,0.5));
    m_pGirdView->setPosition(Vec2(298,170));
    m_pGridViewBg->addChild(m_pGirdView,7);
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(1);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SnowStepLayer4::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-1500 , 0));
    
    LayerColor* layer = LayerColor::create(Color4B(0,0,0,180));
    layer->setTag(100);
    this->addChild(layer,5);
    
    
    m_pBack = ToolSprite::create("images/ice_cream/3/popup_btn_back.png");
    m_pBack->setDelegate(this);
    m_pBack->setTag(101);
    m_pBack->setAnchorPoint(Vec2(0.5,0.5));
    m_pBack->setIsMove(false);
    m_pBack->setPosition(Vec2(25,175));
    m_pGridViewBg->addChild(m_pBack);
    
    m_pNext = ToolSprite::create("images/ice_cream/3/popup_btn_next.png");
    m_pNext->setDelegate(this);
    m_pNext->setTag(201);
    m_pNext->setAnchorPoint(Vec2(0.5,0.5));
    m_pNext->setIsMove(false);
    m_pNext->setPosition(Vec2(571,175));
    m_pGridViewBg->addChild(m_pNext);
    
  
    //DEBUG
//    G_Snow_CupNum = 4;
    
    m_pCup = Sprite::create(__String::createWithFormat("images/snow_cone/2/cup/cup%d.png",G_Snow_CupNum)->getCString());
    kAdapterScreen->setExactPosition(m_pCup, 320, 0 + 200);
    this->addChild(m_pCup,4);
    
    m_pIceCup = Sprite::create("images/snow_cone/3/coneice4.png");
    m_pIceCup->setAnchorPoint(Vec2(0.5,0));
    m_pIceCup->setPosition(Vec2(206,404));
    m_pCup->addChild(m_pIceCup);

    
    
    Rect rectCup = Rect(m_pCup->getPositionX()-132,m_pCup->getPositionY() + 243 - 100, 274, 98 + 300);

    m_pKnife = ToolSprite::create("images/snow_cone/4/knife.png");
    m_pKnife ->setDelegate(this);
    m_pKnife ->cancelEvent(true);
    m_pKnife->setTag(301);
    m_pKnife->setResponsePoint(Vec2(0.5,0.5));
    m_pKnife ->addRect(rectCup);
    kAdapterScreen->setExactPosition(m_pKnife, 320 + 1000, 480);
    this->addChild(m_pKnife,10);
    
    m_pGridViewBg->setPositionY(m_pGridViewBg->getPositionY() + 1000);
    
    
    iceParticle = ParticleSystemQuad::create("particle/shaveSnow.plist");
    iceParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    iceParticle->setVisible(false);
    iceParticle->setPosition(m_pIceCup->getContentSize()*.5);
    m_pIceCup->addChild(iceParticle,4);
    
    
    titleAction();
    
    return true;
}
void SnowStepLayer4::onEnter()
{
    StepLayer::onEnter();
    m_pGirdView->refresh();
    m_pGirdView->setEnable(true);

}
void SnowStepLayer4::onExit()
{
    StepLayer::onExit();
}
void SnowStepLayer4::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    
}
void SnowStepLayer4::titleAction()
{
    
    auto func = CallFunc::create([&](){
        
        
//      m_pGirdView->setScrollEnable(true);
        m_pGirdView->setEnable(true);
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 2);
    });
    
   
    ActionUtils::dropOut(m_pGridViewBg, Vec2(0,-1000),2.0,func);
    
}
void SnowStepLayer4::knifeAction()
{
    
    auto func = CallFunc::create([&](){
        m_pKnife->cancelEvent(false);
    });
  
   
    ActionUtils::moveBy(m_pKnife, Vec2(-1000,0),1.0,func);
    
}

//滑动杯子点击回调
void SnowStepLayer4::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    if (kIAPManager->isFree("shape", index)) {
        
//        m_pGirdView->setScrollEnable(false);
        
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
        
        m_pCurrentSprite = nullptr;
        
        G_Snow_ShapeNum = index + 1;
        
        auto func = CallFunc::create([&](){
            
            knifeAction();
            
            m_pGridViewBg->setVisible(false);
            this->getChildByTag(100)->setVisible(false);
            
        });
        
        
        
        ActionUtils::shakeCup(item, 0, 3,func);
        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }
    
    
    
    
   
    
}
void SnowStepLayer4::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == 101) {
        //back
        toolSprite->setTexture("images/ice_cream/3/popup_btn_back_h.png");
        m_pGirdView->movePreStep();

        
    }else if(tag == 201){
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_next_h.png");
        m_pGirdView->moveNextStep();
        
    
    }else if(tag == 301){
    
        AudioHelp::getInstance()->playLoofEffect("snowcone.mp3");
    
    }
}
void SnowStepLayer4::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    int tag = toolSprite->getTag();
    if (tag == 101) {
        //back
        toolSprite->setTexture("images/ice_cream/3/popup_btn_back.png");
        
        
        
    }else if(tag == 201){
        
        toolSprite->setTexture("images/ice_cream/3/popup_btn_next.png");
        
    }else if(tag == 301){
        
        iceParticle->setVisible(false);
        AudioHelp::getInstance()->stopLoofEffect();
    }

}
void SnowStepLayer4::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{

}
void SnowStepLayer4::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    static int n = 0;
    if(abs(pTouch->getDelta().y) > 30){
        n++;
        
        iceParticle->setVisible(true);
        
        
        if (n == 50) {
            
            AudioHelp::getInstance()->stopLoofEffect();
            m_pIceCup->setTexture(__String::createWithFormat("images/snow_cone/4/shape%d.png",G_Snow_ShapeNum)->getCString());
            n = 0;
            iceParticle->removeFromParent();
            toolSprite->cancelEvent(true);
            toolSprite->setVisible(false);
            
            
            auto particle = ParticleSystemQuad::create("particle/snowCone.plist");
            particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            particle->setVisible(true);
            particle->setPosition(Vec2(400,0));
            this->addChild(particle,4);
            
            AudioHelp::getInstance()->playEffect("hurrah.mp3");
//            m_pGameLayer->doNextStep(SnowStepLayer5::create(m_pGameLayer));
            nextStep();
        }
    }
}
void SnowStepLayer4::nextStep()
{
//    m_pGameLayer->nextButtonClicked(SnowStepLayer5::create(m_pGameLayer));
    AudioHelp::getInstance()->stopLoofEffect();
    auto func = CallFunc::create([=](){
        
        //next
//        m_pGameLayer->doNextStep(SnowStepLayer5::create(m_pGameLayer));
        m_pGameLayer->nextButtonClicked(SnowStepLayer5::create(m_pGameLayer));
    });
    ActionUtils::delayTimeAction(this,3.0,func);
}
void SnowStepLayer4::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void SnowStepLayer4::onNegativeClick(void* type)
{
    isReturn = false;
}
void SnowStepLayer4::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  SnowStepLayer4::touchEvent(Ref *obj , Widget::TouchEventType type)
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