//
//  SnowStepLayer5.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/8.
//
//

#include "SnowStepLayer5.h"
#include "Global.h"
#include "IAPManager.h"
#include "STAds.h"
#include "ActionUtils.h"
#include "SnowDecorationLayer.h"

int h[10] = {-129,154,160,-72,-139,15,180,-29,166, 89};
int s[10] = {  -7,-27, 25,-16,  27,18, 25,-20, 22,-19};
int v[10] = {  33, -7,  2,  1,  -8, 2, 34,  0, 38, 17};
SnowStepLayer5::SnowStepLayer5(GameLayer* gameLayer):StepLayer(gameLayer)
{
    
}
SnowStepLayer5::~SnowStepLayer5()
{
    if (m_vSyrupPaint.size()>0) {
        for (int i = 0; i<m_vSyrupPaint.size(); i++) {
            delete m_vSyrupPaint.at(i);
        }
        m_vSyrupPaint.clear();
    }
    
}
SnowStepLayer5* SnowStepLayer5::create(GameLayer* gameLayer)
{
    SnowStepLayer5* step1 = new SnowStepLayer5(gameLayer);
    
    if(step1&&step1->init())
    {
        step1->autorelease();
        return step1;
    }
    
    CC_SAFE_DELETE(step1);
    return nullptr;
    
}
bool SnowStepLayer5::init()
{
    bool isInit = false;
    do {
        CC_BREAK_IF(!StepLayer::init());
        
        initData();
        
        m_bClick = true;
        m_bNext = true;
        
        CC_BREAK_IF(!initUI());
        
        
        
        isInit = true;
        
    } while (0);
    
    return isInit;
    
    
    
}
void SnowStepLayer5::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<10; i++) {

        __String* str = __String::createWithFormat("images/snow_cone/5/icon_syrup/icon_syrup%d.png",i);
        Sprite* sprite = Sprite::create(str->getCString());
        
//        __String* strData = __String::create("syrupcup");
//        sprite->setUserData((char*)strData->getCString());
        
        sprite->setScale(0.7);
        m_vTypes.pushBack(sprite);
        
    }
    
}
bool SnowStepLayer5::initUI()
{
    Sprite* bg = Sprite::create("images/snow_cone/4/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
    
    Sprite* bg1 = Sprite::create("images/snow_cone/4/bg1.png");
    kAdapterScreen->setExactPosition(bg1, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(bg1,20);
    
    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width + 20, 400));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
        m_pGirdView->m_fHSpace = 30;
    //    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 5;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
//    m_pGirdView->setScrollEnable(false);
    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 0 - 100,Vec2(0,1),kBorderLeft,kBorderTop);
    this->addChild(m_pGirdView,7);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(2);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SnowStepLayer5::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;

    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-5200, 0));
    
  
    m_pCup = Sprite::create(__String::createWithFormat("images/snow_cone/2/cup/cup%d.png",G_Snow_CupNum)->getCString());
    kAdapterScreen->setExactPosition(m_pCup, 320, 0 + 200);
    this->addChild(m_pCup,4);
    
    G_Snow_ShapeIce = Sprite::create(__String::createWithFormat("images/snow_cone/4/shape%d.png",G_Snow_ShapeNum)->getCString());
    G_Snow_ShapeIce->setAnchorPoint(Vec2(0.5,0.5));
    kAdapterScreen->setExactPosition(G_Snow_ShapeIce, 320, 370 + 200);
    this->addChild(G_Snow_ShapeIce,5);

    G_Snow_ShapeIce->retain();
    
    Rect rectCup = Rect(m_pCup->getPositionX()-132 - 100,m_pCup->getPositionY()+243 - 150, 274 + 200, 98 + 400);

    
    m_pSyrup = ToolSprite::create("images/snow_cone/5/syrup/syrup0.png");
    m_pSyrup->setAnchorPoint(Vec2(0,0));
    kAdapterScreen->setExactPosition(m_pSyrup, 320, 480 + 200);
    m_pSyrup->setResponsePoint(Vec2(0,0));
    m_pSyrup->setDelegate(this);
    m_pSyrup->setRotation(60);
    m_pSyrup->setIsMove(true);
//    m_pSyrup->addRect(rectCup,this);
    m_pSyrup->setMoveRect(Rect::ZERO);
//    m_pSyrup->setVisible(false);
    m_pSyrup->cancelEvent(false);
    m_pSyrup->addRectByNode(this);
    this->addChild(m_pSyrup,15);
    m_pSyrup->setScale(0);
    
    
    titleAction();
    
    return true;
}
void SnowStepLayer5::onEnter()
{
    StepLayer::onEnter();
    if (m_pGirdView) {
        
         m_pGirdView->refresh();
        
    }
    m_pGirdView->setEnable(true);

  

}
void SnowStepLayer5::onExit()
{
    StepLayer::onExit();
}
void SnowStepLayer5::onEnterTransitionDidFinish()
{
    StepLayer::onEnterTransitionDidFinish();
    
    
}
void SnowStepLayer5::titleAction()
{
    //     ActionUtils::dropOut(m_pTitle, Vec2(0,-400),2.0);
    
    
    auto func = CallFunc::create([&](){
        
        
//        m_pGirdView->setScrollEnable(true);
        m_pGirdView->setEnable(true);
        knifeAction();
        
        
    });
    
    m_pCup->runAction(MoveBy::create(0.5, Vec2(0,-200)));
    ActionUtils::moveBy(G_Snow_ShapeIce, Vec2(0,-200),0.5);
    ActionUtils::delayTimeAction(this,1.0,func);
    
}
void SnowStepLayer5::knifeAction()
{
    auto func = CallFunc::create([&](){
        
//        m_pGirdView->setScrollEnable(true);
        
    });
    
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(50, 0), 2);
   
    ActionUtils::delayTimeAction(this,1.0,func);
    
}
void SnowStepLayer5::clickAction()
{
    auto func = CallFunc::create([&](){
        
        m_bClick=true;
        
    });
    ActionUtils::delayTimeAction(this,0.1,func);
}
//滑动杯子点击回调
void SnowStepLayer5::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_pGirdView->setEnable(false);
    if (kIAPManager->isFree("syrupcup", index)) {
        
//        m_pGirdView->setScrollEnable(false);
        
        log("click");
        
        if (m_bNext) {
            
            m_bNext = false;
            m_pGameLayer->m_bNextAction = false;
            m_pGameLayer->OperateEndToDecoration(SnowDecorationLayer::create(m_pGameLayer));
            
        }
        
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
        
        
        __String* fileName = __String::createWithFormat("images/snow_cone/4/color/color%d.png",G_Snow_ShapeNum);
        
//        m_pSyrup->setVisible(true);
//        m_pSyrup->cancelEvent(false);
        
        m_pSyrup->setTexture(__String::createWithFormat("images/snow_cone/5/syrup/syrup%d.png",index)->getCString());
        
        
        m_pSyrup->setScale(0);
        m_pSyrup->setPosition(item->getParent()->convertToWorldSpace(item->getPosition()));
        
        ScaleTo* scale = ScaleTo::create(0.5, 1);
        MoveTo* move = MoveTo::create(0.5, kAdapterScreen->getExactPostion(Vec2(480,320 + 200)));
        Spawn* sp = Spawn::create(scale,move, NULL);
        
        auto func = CallFunc::create([&](){
            
            auto particle = ParticleSystemQuad::create("particle/decorateParticle.plist");
            particle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
            particle->setVisible(true);
            particle->setPosition(m_pSyrup->getParent()->convertToWorldSpace(m_pSyrup->getPosition() + Point(0,0)));
            this->addChild(particle,20);
            
            AudioHelp::getInstance()->playEffect("all star .mp3");
            
//            m_pGirdView->setScrollEnable(true);
            m_pGirdView->setEnable(true);
            
        });
        m_pSyrup->runAction(Sequence::create(sp,func, NULL));
        
        auto pSprite2_ = Sprite::createWithTexture(ActionUtils::getShapeScribbleTexture(fileName, h[index], s[index], v[index]));
        pSprite2_->setPosition(Point(pSprite2_->getContentSize().width / 2, pSprite2_->getContentSize().height / 2));
        
        Size lCandyCoatingSize = pSprite2_->getTexture()->getContentSizeInPixels();
        Point lCandyCoatingPosition = Vec2(317, 670-80-80);
        _rtx = RenderTexture::create(lCandyCoatingSize.width, lCandyCoatingSize.height);
        _rtx->setPosition(Vec2(200 + 80 - 1,200+ 54));
        _rtx->setAnchorPoint(Vec2(0,0));
        G_Snow_ShapeIce->addChild(_rtx);
        
        auto paint =new Scribble(Sprite::create("images/snow_cone/solid_brush.png"));
        paint->setBrushShader();
        paint->setBrushType(Scribble::eBrush);
        paint->setTargetSolid(true);
        paint->setTargetAlphaTestValue(0.0f);
        Sprite *lBrush = Sprite::create("images/snow_cone/solid_brush.png");
        paint->setBrush(lBrush->getTexture());
        paint->setTarget(pSprite2_);
        m_vSyrupPaint.push_back(paint);

        
    }else{
        
        SceneManager::getInstance()->enterShopScene();
    }
    
}
void SnowStepLayer5::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    log("click  x:%f  y:%f",pTouch->getLocation().x,pTouch->getLocation().y);
    log("click  x:%f  y:%f",toolSprite->getPosition().x,toolSprite->getPositionY());
    AudioHelp::getInstance()->playLoofEffect("snowcone.mp3");
    
}
void SnowStepLayer5::onTouchUp(ToolSprite* toolSprite,Touch *pTouch)
{
   
    
    AudioHelp::getInstance()->stopLoofEffect();
    
}
void SnowStepLayer5::onTouchMove(ToolSprite* toolSprite,Touch *pTouch)
{
    
    
  
}
void SnowStepLayer5::onTouchMoveInRect(ToolSprite* toolSprite,Touch *pTouch,const Rect& rect,int index)
{
    log("move  x:%f  y:%f",pTouch->getLocation().x,pTouch->getLocation().y);
    
    auto cakePaint = dynamic_cast<Scribble*>( m_vSyrupPaint.back());
    cakePaint->paint(nullptr,_rtx,Vec2(toolSprite->getPositionX()-300*0.86 + 120,toolSprite->getPositionY() - 65));
}
void SnowStepLayer5::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
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
void SnowStepLayer5::onNegativeClick(void* type)
{
    isReturn = false;
}
void SnowStepLayer5::onPositiveClick(void* type)
{
    SceneManager::getInstance()->popToRootScene();
    
    if (kIAPManager->isShowAds()) {
        STAds ads;
        ads.preloadCrosspromo();
    }
    
}
void  SnowStepLayer5::touchEvent(Ref *obj , Widget::TouchEventType type)
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