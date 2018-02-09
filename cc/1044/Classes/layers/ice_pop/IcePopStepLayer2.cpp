//
//  IcePopStepLayer2.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/6.
//
//

#include "IcePopStepLayer2.h"
#include "IcePopStepLayer3.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"


Color4F color[17] ={Color4F((float)245/255,(float)64/255,(float)61/255,1.0),
Color4F((float)253/255,(float)81/255,(float)90/255,1.0),
    Color4F((float)255/255,(float)240/255,(float)0/255,1.0),
    Color4F((float)65/255,(float)242/255,(float)31/255,1.0),
    Color4F((float)255/255,(float)255/255,(float)241/255,1.0),
    Color4F((float)96/255,(float)35/255,(float)17/255,1.0),
    Color4F((float)44/255,(float)223/255,(float)255/255,1.0),
    Color4F((float)255/255,(float)151/255,(float)2/255,1.0),
    Color4F((float)90/255,(float)0/255,(float)118/255,1.0),
    Color4F((float)204/255,(float)2/255,(float)255/255,1.0),
    Color4F((float)255/255,(float)123/255,(float)150/255,1.0),
    Color4F((float)0/255,(float)255/255,(float)228/255,1.0),
    Color4F((float)255/255,(float)0/255,(float)253/255,1.0),
    Color4F((float)112/255,(float)2/255,(float)1/255,1.0),
    Color4F((float)242/255,(float)113/255,(float)131/255,1.0),
    Color4F((float)253/255,(float)159/255,(float)209/255,1.0),
};
IcePopStepLayer2::IcePopStepLayer2(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
IcePopStepLayer2::~IcePopStepLayer2()
{
    
}
IcePopStepLayer2* IcePopStepLayer2::create(GameLayer* gameLayer)
{
    IcePopStepLayer2* step1 = new IcePopStepLayer2(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool IcePopStepLayer2::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        initData();
        
//        m_iTasteType＝0;
        m_iTasteType = -1;
        
        m_bIsFirst = true;
        
        m_iTimeTag = 0;
        //口味进度条的百分数
        m_fTimePercentage = 0;
        
        m_pCurrentSprite=nullptr;
        
        m_bIsShow = true;
        
        CC_BREAK_IF(!initUI());
        isInit = true;
        
    } while (0);
    
    return isInit;
}
void IcePopStepLayer2::initData()
{
    m_vTypes.clear();
    for (int i = 1; i<17; i++) {

        __String* str = __String::createWithFormat("images/ice_pop/2/fruit_juice/%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
//        __String* strData = __String::create("taste");
//        sprite->setUserData((char*)strData->getCString());

        
        
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool IcePopStepLayer2::initUI()
{
    Sprite* bg = Sprite::create("images/ice_pop/2/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 130));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
        m_pGirdView->m_fHSpace = 20;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 200 + 30;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 266 + 500,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,7);
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-5200, 0));
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(7);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IcePopStepLayer2::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;

    
    Sprite* pipe = Sprite::create("images/ice_pop/2/pipe1.png");
    kAdapterScreen->setExactPosition(pipe, 320, 266 + 500,Vec2(0.5,1),kBorderNone,kBorderBottom);
    pipe->setTag(122);
    this->addChild(pipe);
    pipe->setScale(0);
    
    m_pSwitch = ToolSprite::create("images/ice_pop/2/pipe_switch_1.png");
    m_pSwitch->setPosition(Vec2(15 + 35,47));
    m_pSwitch->setAnchorPoint(Vec2(0.198,0.75));
    m_pSwitch->setDelegate(this);
    m_pSwitch->setResponsePoint(Vec2(0.5,0.5));
    m_pSwitch->setIsMove(false);
    m_pSwitch->cancelEvent(true);
    pipe->addChild(m_pSwitch);
  
    
//    waterParticle->retain();
    
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
//
//
    
    
    return true;
}
//利用遮罩，根据冰棍形状来绘制冰棍口味的形状
void IcePopStepLayer2::initMoule()
{
    m_vTypesTaste.clear();
    
    Sprite* brush = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/brush%d.png",G_IcePop_MouleNum)->getCString());
    
    for (int i = 1;i<17;i++) {
        
        __String* str = __String::createWithFormat("images/ice_pop/2/taste/%d.png",i);
        Sprite* mouleBg = Sprite::create(str->getCString());
        
        RenderTexture *tasteTexture = RenderTexture::create(brush->getContentSize().width, brush->getContentSize().height, Texture2D::PixelFormat::RGBA8888);
        tasteTexture->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
        
        BlendFunc ccbFunc = {GL_ZERO,GL_ONE_MINUS_SRC_ALPHA};
        brush->setBlendFunc(ccbFunc);
        brush->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
        
        tasteTexture->begin();
        
        mouleBg->setPosition(Point(brush->getContentSize().width/2.0, brush->getContentSize().height/2.0));
        
        mouleBg->visit();
        brush->visit();
        tasteTexture->end();
        
        Director::getInstance()->getRenderer()->render();
        
        m_vTypesTaste.pushBack(tasteTexture->getSprite());
    }

}
void IcePopStepLayer2::onEnter()
{
    StepLayer::onEnter();
    m_pGirdView->refresh();
    m_pGirdView->setEnable(true);
    
    if (m_bIsFirst) {
        m_bIsFirst=false;
        G_IcePop_MouleSprite = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
        kAdapterScreen->setExactPosition(G_IcePop_MouleSprite, 290 + 30, 100,Vec2(0.5,0),kBorderNone,kBorderBottom);
        //保留G_pMoule，注意释放release
        G_IcePop_MouleSprite->retain();
        this->addChild(G_IcePop_MouleSprite ,15);
        
        initMoule();
        
        Sprite* moule = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
        kAdapterScreen->setExactPosition(moule, 290 + 30, 100,Vec2(0.5,0),kBorderNone,kBorderBottom);
        this->addChild(moule,20);
        
        
        chooseAction();
    }
   
}
void IcePopStepLayer2::onExit()
{
    StepLayer::onExit();
}
void IcePopStepLayer2::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
  

}
void IcePopStepLayer2::titleAction()
{
    auto func = CallFunc::create([&](){
        
        
        m_pGirdView->setEnable(true);
    });
    
    ActionUtils::delayTimeAction(this,1.0,func);
    
}
void IcePopStepLayer2::chooseAction()
{
    
    
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 2);
    auto func = CallFunc::create([&](){
        
        titleAction();
        
    });
    ActionUtils::delayTimeAction(this,2,func);
    
}

//滑动杯子点击回调
void IcePopStepLayer2::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    if(m_pCurrentSprite){
        
        m_pCurrentSprite->removeFromParent();
        m_pCurrentSprite = nullptr;
        
    }
    m_pGirdView->setEnable(false);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if (kIAPManager->isFree("taste", index)) {
      m_pGirdView->setEnable(true);
        m_pSwitch->cancelEvent(false);
        
        if (this->getChildByTag(122)) {
            
            if (m_bIsShow) {
                m_bIsShow = false;
                this->getChildByTag(122)->runAction(ScaleTo::create(0.5, 1));
            }
            
            
        }
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        chooseParticle->setContentSize(item->getContentSize());
        item->addChild(chooseParticle,4);
        AudioHelp::getInstance()->playEffect("choose.mp3");
        
        
        
            auto pitch = Sprite::create("images/ice_pop/2/pitch_on.png");
            pitch->setPosition(Vec2::ZERO);
            pitch->setAnchorPoint(Vec2::ZERO);
  
            item->addChild(pitch);
            
            
            
            m_pCurrentSprite = pitch;
            
            
            m_iTasteType = index;
            m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-(203)*index + 40,0), 0.5);
            
        
        
        
    }else{
        SceneManager::getInstance()->enterShopScene();
        m_iTasteType = -1;
        
    }
    
   
    
    
}
void IcePopStepLayer2::updateMoule(float f)
{
    if(G_IcePop_MouleSprite->getChildByTag(m_iTimeTag)){
        
        dynamic_cast<ProgressTimer*>(G_IcePop_MouleSprite->getChildByTag(m_iTimeTag))->setPercentage(dynamic_cast<ProgressTimer*>(G_IcePop_MouleSprite->getChildByTag(m_iTimeTag))->getPercentage()+1.5);
        
    }

}
void IcePopStepLayer2::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    log("%d",m_iTasteType);
    
    if (m_iTasteType == -1) {
        return;
    }
    
    
    AudioHelp::getInstance()->playEffect("pour_click.mp3");
    
    if(m_iTasteType != -1 && m_fTimePercentage < 99.8)
    {
        m_iTimeTag++;
        
        log("---%d",m_iTasteType);
        log("---%d",m_iTimeTag);
        log("---%f",m_fTimePercentage);
        
        ProgressTimer* mouleTimer = ProgressTimer::create(m_vTypesTaste.at(m_iTasteType));
        mouleTimer->setType(ProgressTimer::Type::BAR);
        mouleTimer->setBarChangeRate(Point(0,1));
        mouleTimer->setMidpoint(Point(0.5,0));
        mouleTimer->setPosition(Vec2::ZERO);
        mouleTimer->setPercentage(m_fTimePercentage);
        mouleTimer->setAnchorPoint(Vec2::ZERO);
        mouleTimer->setTag(m_iTimeTag);
        G_IcePop_MouleSprite->addChild(mouleTimer,100 - m_iTimeTag);
        
    }
   
    waterParticle = nullptr;
    waterParticle = ParticleSystemQuad::create("particle/waterDown.plist");
    waterParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    waterParticle->setVisible(true);
    
    waterParticle->setRotation(180);
    waterParticle->setScaleY(0.7);
    
    

    if (m_iTasteType == 14 || m_iTasteType == 15 ) {
        this->schedule(schedule_selector(IcePopStepLayer2::updatePartic),0.5f);
    }else{
        waterParticle->setStartColor(color[m_iTasteType]);
        //    waterParticle->setStartColor(Color4F(255/255,0,0,255/255));
        waterParticle->setEndColor(color[m_iTasteType]);
    
    }
    
    
    kAdapterScreen->setExactPosition(waterParticle, 320, 266 + 500 - 115 - 50,Vec2(0.5,1),kBorderNone,kBorderBottom);
    this->addChild(waterParticle,4);
    
    toolSprite->setTexture("images/ice_pop/2/pipe_switch_2.png");

    this->schedule(schedule_selector(IcePopStepLayer2::updateMoule),0.3f);
    
    AudioHelp::getInstance()->playLoofEffect("pour liquid.mp3");
    
}
void IcePopStepLayer2::updatePartic(float f)
{
    static int i = 1;
    i++;
    if (waterParticle&&m_iTasteType == 14) {
        if (i%2 == 0) {
            waterParticle->setStartColor(color[6]);
            
            waterParticle->setEndColor(color[6]);
        }
        if(i%3 == 0){
            waterParticle->setStartColor(color[10]);
            
            waterParticle->setEndColor(color[10]);
        }
        
    }else if(waterParticle&&m_iTasteType == 15){
        if (i%2 == 0) {
            waterParticle->setStartColor(color[6]);
            
            waterParticle->setEndColor(color[6]);
        }
        if(i%3 == 0){
            waterParticle->setStartColor(color[2]);
            
            waterParticle->setEndColor(color[2]);
        }
        if(i%4 == 0){
            waterParticle->setStartColor(color[1]);
            
            waterParticle->setEndColor(color[1]);
        }
        if(i%5 == 0){
            waterParticle->setStartColor(color[3]);
            
            waterParticle->setEndColor(color[3]);
        }
        if(i%6 == 0){
            waterParticle->setStartColor(color[9]);
            
            waterParticle->setEndColor(color[9]);
        }
        
    
    }
    

}
void IcePopStepLayer2::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
    AudioHelp::getInstance()->stopLoofEffect();
  
    toolSprite->setTexture("images/ice_pop/2/pipe_switch_1.png");
    this->unschedule(schedule_selector(IcePopStepLayer2::updateMoule));
    if (waterParticle) {
        waterParticle->removeFromParent();
        waterParticle=nullptr;
    }
    if (m_iTasteType == 15) {
        this->schedule(schedule_selector(IcePopStepLayer2::updatePartic),0.3f);
    }
    if(G_IcePop_MouleSprite->getChildByTag(m_iTimeTag)){
        
        m_fTimePercentage = dynamic_cast<ProgressTimer*>(G_IcePop_MouleSprite->getChildByTag(m_iTimeTag))->getPercentage();
    
        
        log("m_fTimePercentage---%f",m_fTimePercentage);
        if (m_fTimePercentage > 99.5) {
            
            m_pSwitch->cancelEvent(true);
            if (waterParticle) {
                waterParticle->removeFromParent();
            }
            
            
            auto particle = ParticleSystemQuad::create("particle/icePop.plist");
            particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            particle->setVisible(true);
            particle->setPosition(Vec2(400,0));
            this->addChild(particle,20);
            
            AudioHelp::getInstance()->playEffect("hurrah.mp3");
            
//            m_pGameLayer->doNextStep(IcePopStepLayer3::create(m_pGameLayer));
            nextStep();
//            m_pGirdView->setScrollEnable(false);
            m_pGirdView->setEnable(false);
        }
        
        
    }
    
    
}
void IcePopStepLayer2::nextStep()
{
    auto func = CallFunc::create([&](){
        
        //next
        m_pGameLayer->m_bNextAction = true;
        m_pGameLayer->doNextStep(IcePopStepLayer3::create(m_pGameLayer));
    });
    ActionUtils::delayTimeAction(this,3.0,func);
}
void IcePopStepLayer2::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    
    
}
void IcePopStepLayer2::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void IcePopStepLayer2::onNegativeClick(void* type)
{
    isReturn = false;
}
void IcePopStepLayer2::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  IcePopStepLayer2::touchEvent(Ref *obj , Widget::TouchEventType type)
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
