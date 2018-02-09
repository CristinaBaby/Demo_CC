//
//  IcePopDecorationLayer.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#include "IcePopDecorationLayer.h"
#include "IAPManager.h"
#include "AdapterScreen.h"
#include "Global.h"
#include "ToolSprite.h"
#include "ScribbleTouchNode.h"
#include "AudioHelp.h"
#include "CFAds.h"
#include "Global.h"
#include "MyPaintSprite.h"
#include "STAds.h"
#include "FillMaterialModel.h"

enum DecorateType
{
    FROSTING = 0,
    CANDY,
    NUTS,
    EXTRA,
    FRUIT,
    DRINK,
    SCENES,
};
IcePopDecorationLayer::IcePopDecorationLayer(GameLayer* gameLayer)
{
    m_pGameLayer = gameLayer;
    decorationLayer = CCLayer::create();
    decorationLayer->retain();
    
    fillDecorationLayer = CCLayer::create();
    fillDecorationLayer->retain();

}
IcePopDecorationLayer::~IcePopDecorationLayer()
{
    log("G_IcePop_MouleSprite----%d",G_IcePop_MouleSprite->getReferenceCount());
    
    CC_SAFE_RELEASE_NULL(decorationLayer);
    CC_SAFE_RELEASE_NULL(fillDecorationLayer);
    CC_SAFE_RELEASE_NULL(G_IcePop_MouleSprite);
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "purchase_Finish");
}

IcePopDecorationLayer* IcePopDecorationLayer::create(GameLayer* gameLayer)
{
    IcePopDecorationLayer* dec = new IcePopDecorationLayer(gameLayer);
    
    if(dec&&dec->init())
    {
        dec->autorelease();
        return dec;
    }
    CC_SAFE_DELETE(dec);
    return nullptr;
}


void IcePopDecorationLayer::onEnter()
{
    BaseLayer::onEnter();
    m_pGameLayer->isDecorateEnd = true;
    m_pGameLayer->showNextButton(true);
    m_pUnitScrollView->setEnable(true);
    kAdapterScreen->setExactPosition(m_pGameLayer->m_pNextItemSprite, 10, 10 + 100 + 150,Vec2(1, 0),kBorderRight,kBorderBottom);
    
    //DEBUG
//    G_IcePop_MouleNum = 5;
//    auto ice = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
//    kAdapterScreen->setExactPosition(ice, 320, 350 + 50 + 100);
//    this->addChild(ice,4);
//    ice->setRotation(180);
    
    
    if (m_bIsFirst) {
        m_bIsFirst=false;
        if (G_IcePop_MouleSprite) {
            //        G_IcePop_MouleSprite->removeAllChildren();
            G_IcePop_MouleSprite->removeFromParent();
            kAdapterScreen->setExactPosition(G_IcePop_MouleSprite, 320, 350 + 50 + 100 );
            this->addChild(G_IcePop_MouleSprite,3);
            G_IcePop_MouleSprite->setScale(1);
            
            
            auto ice = Sprite::create(__String::createWithFormat("images/ice_pop/2/moule/mould_%d.png",G_IcePop_MouleNum)->getCString());
            kAdapterScreen->setExactPosition(ice, 320, 350 + 50 + 100);
            this->addChild(ice,4);
            ice->setRotation(180);
            //        ice->setPosition(Vec2(200,200));
            //        ice->setAnchorPoint(Vec2::ZERO);
            //        G_IcePop_MouleSprite->addChild(ice,20);
            
            
            
            m_pSticks = Sprite::create("images/ice_pop/3/stick.png");
            kAdapterScreen->setExactPosition(m_pSticks, 320, 300 + 50);
            this->addChild(m_pSticks,2);
            
        }

    }
}
void IcePopDecorationLayer::onExit()
{
    BaseLayer::onExit();
    G_IcePop_MouleSprite->setVisible(true);
    for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
        
        dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
        
    }
    
}
void IcePopDecorationLayer::showBanner(Ref* r)
{
    
    log("bannerDidShow11111");
    
}
void IcePopDecorationLayer::hideBanner(Ref* r)
{
    
}
bool IcePopDecorationLayer::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        m_pIndex=-1;
        m_bIsTouch = false;
        m_bIsScrollBgShow = false;
        m_bPaintTouch = false;
        m_bIsFirst = true;
        isInit=true;
       
        m_pUnitSprite = nullptr;
        m_iFrontIndex = -1;
        m_iBackrontIndex = -1;
        
        m_vCoasting.clear();
        
    }while(0);
    
    return isInit;
    
}
bool IcePopDecorationLayer::initUI()
{
    this->addToContentLayer(decorationLayer, 5);
    
    this->addToContentLayer(fillDecorationLayer, 12);
    
    pBgSprite = Utils::getUISprite("images/ice_pop/dec/bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 320, 480);
    this->addToBackGroundLayer(pBgSprite, 1);
    
    auto pBg = Utils::getUISprite("images/ice_pop/dec/bg1.png");
    kAdapterScreen->setExactPosition(pBg, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToBackGroundLayer(pBg, 2);
    
    m_pCup = nullptr;
    
    m_pUnitScrollbgsprite = Utils::getUISprite("images/dec/dec_bar.png");
    kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,320 + 1000,100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    this->addToUILayer(m_pUnitScrollbgsprite, 5);
    //    m_pUnitScrollbgsprite->setVisible(false);
    
    m_pGirdView = KDGridView::create(Size(470, 150));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    //m_pGirdView->m_iRow = 4;
    m_pGirdView->m_fHSpace = 10;
    //    m_pGirdView->m_fVSpace = (kAdapterScreen->screenSize.height - 129 *4) /5;
    //    m_pGirdView->m_fMarginTop = (kAdapterScreen->screenSize.height - 129 * 4) /5;
    m_pGirdView->m_fMarginRight = 50;
    kAdapterScreen->setExactPosition(m_pGirdView, 320, 30 + 70,Vec2(0.5,0.5),kBorderNone,kBorderTop);
    this->addToUILayer(m_pGirdView,15);
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setType(1);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IcePopDecorationLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    //m_pGirdView->m_fMarginBottom = 200;
    
    
    m_pUnitScrollView = UnitScrollView::create(Size(607, 180));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(IcePopDecorationLayer::onUnitCallback, this));
//    kAdapterScreen->setExactPosition(m_pUnitScrollView,0 + 15,100,Vec2(0, 0),kBorderLeft,kBorderBottom);
    m_pUnitScrollView->setAnchorPoint(Vec2(0.5,0.5));
    m_pUnitScrollView->setPosition(Vec2(m_pUnitScrollbgsprite->getContentSize().width/2,m_pUnitScrollbgsprite->getContentSize().height/2 ));
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pUnitScrollbgsprite->addChild(m_pUnitScrollView,20);
    
    m_pUnitScrollView->setZOrder(8);
    //    m_pUnitScrollView->m_iCol = 1;
    m_pUnitScrollView->m_iRow = 1;
    m_pUnitScrollView->m_fHSpace = -50;
    m_pUnitScrollView->m_fVSpace = 20;
    m_pUnitScrollView->m_bNeedAdjust = false;
    m_pUnitScrollView->m_fMarginTop = 10 - 40;
    
    resetBtn = ui::Button::create("images/button/btn_reset.png");
    resetBtn -> setAnchorPoint(Vec2(1, 1));
    
    log("%f",bannerHeight);
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    kAdapterScreen->setExactPosition(resetBtn, 10, 10 + 100 + 150,Vec2(0, 0),kBorderLeft,kBorderBottom);
#else
    kAdapterScreen->setExactPosition(resetBtn, 10, 10 + 100 + 150,Vec2(0, 0),kBorderLeft,kBorderBottom);
#endif
    resetBtn->setTag(51);
    this->addToUILayer(resetBtn, 10);
    resetBtn -> addTouchEventListener(CC_CALLBACK_2(IcePopDecorationLayer::touchEvent, this));
    
    m_pGameLayer->m_bNextAction = false;
    m_pGameLayer->showNextButton(true);
    
//    m_pUnitScrollbgsprite->setPositionX(m_pUnitScrollbgsprite->getPositionX()+1000);
    m_pGirdView->setPositionX(m_pGirdView->getPositionX()+1000);
    
    startAnimtion();
    
    EventListenerTouchOneByOne* e = EventListenerTouchOneByOne::create();
    e->onTouchBegan = CC_CALLBACK_2(IcePopDecorationLayer::TouchBegan, this);
    e->onTouchEnded = CC_CALLBACK_2(IcePopDecorationLayer::TouchEnded, this);
    e->onTouchMoved = CC_CALLBACK_2(IcePopDecorationLayer::TouchMoved, this);
    e->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    
   
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IcePopDecorationLayer::purchaseFinish), "purchase_Finish", NULL);
    
    return true;
}
bool IcePopDecorationLayer::TouchBegan(Touch *pTouch, Event *pEvent)
{
   
    if (!m_bIsTouch) {
        return false;
    }
    
    if (this->getChildByTag(102)) {
        this->getChildByTag(102)->removeFromParent();
    }
    
    if(m_iFrontIndex != -1 && m_bPaintTouch){
        
        auto sprite = Sprite::create(__String::createWithFormat("images/dec/category/%s/%s%d.png",G_IcePop_MenuItem[m_iFrontIndex].c_str(),G_IcePop_MenuItem[m_iFrontIndex].c_str(),m_iBackrontIndex)->getCString());
        sprite->setPosition(pTouch->getLocation());
        decorationLayer->addChild(sprite);
        int rote = rand()%360;
        sprite->setRotation(rote);
        
//        ScaleTo* s1 = ScaleTo::create(0.2f,1.08f,0.90f);
//        ScaleTo* s2 = ScaleTo::create(0.2f,0.90,1.08f);
//        ScaleTo* s3 = ScaleTo::create(0.2f,1.0f,1.0f);
//        sprite->runAction(Sequence::create(s1,s2,s3,NULL));
        
        
    }
    return true;
    
}
void IcePopDecorationLayer::TouchEnded(Touch *pTouch, Event *pEvent)
{
    
}
void IcePopDecorationLayer::TouchMoved(Touch *pTouch, Event *pEvent)
{
    if (!m_bPaintTouch) {
        log("move  x:%f  y:%f",pTouch->getLocation().x,pTouch->getLocation().y);
        if (m_vCoasting.size()>0) {
            auto cakePaint = dynamic_cast<MyPaintSprite*>( m_vCoasting.back());
            cakePaint->paint(pTouch->getLocation());
        }
    }
    
    
    
}
void IcePopDecorationLayer::startAnimtion()
{
    
    auto funcSounds=CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("banner drop.mp3");
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5),funcSounds, NULL));
    MoveBy* move = MoveBy::create(1.0,Vec2(-1000,0));
    m_pGirdView->runAction(Sequence::create(DelayTime::create(0.5),move,NULL));
    
    
}
bool  IcePopDecorationLayer::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<5; i++) {
        __String* str = __String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IcePop_MenuItem[i].c_str());
        Sprite* data = Sprite::create(str->getCString());
        m_vTypes.pushBack(data);
    }
    
    return true;
}

void IcePopDecorationLayer::onTypeCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    log("button----->%d",tag);
    
}

void IcePopDecorationLayer::onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    if (tag==0) {
        Vec2 point = m_pGirdView->m_pScrollView->getContentOffset();
        point.y = point.y+150;
        if (point.y<=0) {
            m_pGirdView->m_pScrollView->setContentOffset(point);
        }else{
            point.y = 0;
            m_pGirdView->m_pScrollView->setContentOffset(point);
        }
    }else{
        Vec2 point = m_pGirdView->m_pScrollView->getContentOffset();
        point.y = point.y-150;
        if (point.y>(-m_pGirdView->m_pScrollView->getContentSize().height+m_pGirdView->m_pScrollView->getViewSize().height)) {
            m_pGirdView->m_pScrollView->setContentOffset(point);
        }else{
            point.y = -m_pGirdView->m_pScrollView->getContentSize().height+m_pGirdView->m_pScrollView->getViewSize().height;
            m_pGirdView->m_pScrollView->setContentOffset(point);
        }
    }
    
}

void IcePopDecorationLayer::_onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    
}
void IcePopDecorationLayer::scrollViewTouchBegin(Vec2 worldPoint)
{
    
}
void IcePopDecorationLayer::scrollViewWillScroll()
{
    
}
void IcePopDecorationLayer::hideUnitScroll()
{
    m_pUnitScrollView->setVisible(false);
    auto func1=CallFunc::create([&](){
        //        resetBtn->setVisible(true);
        //        m_pGameLayer->showNextButton(true);
        
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
        
        
        m_bIsScrollBgShow = false;
//        m_pGirdView->setScrollEnable(true);
    });
    
    MoveBy* move = MoveBy::create(0.5, Vec2(1000,0));
    m_pUnitScrollbgsprite->runAction(Sequence::create(move,func1,NULL));
    
    
}
void IcePopDecorationLayer::showUnitScroll(int index)
{
    
    m_pUnitScrollView->loadingUnitScrollViewWithType(index);
    m_pUnitScrollView->setVisible(false);
    auto func=CallFunc::create([&](){
        
        log("===%d",index);
        
        m_pUnitScrollView->setVisible(true);
        m_bIsScrollBgShow = true;

    });
    
    MoveBy* move = MoveBy::create(0.05, Vec2(-1000,0));
    m_pUnitScrollbgsprite->runAction(Sequence::create(move,func,NULL));
    
    
    
}
void IcePopDecorationLayer::purchaseFinish(Ref*)
{
    if (m_pIndex != -1) {
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
    }
    
    
}
//GirdView点击回调
void IcePopDecorationLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    m_bIsTouch = false;
    if(m_pUnitSprite){
        m_pUnitSprite ->removeFromParent();
        m_pUnitSprite=nullptr;
    }
    if (this->getChildByTag(102)) {
        this->getChildByTag(102)->removeFromParent();
    }
    if (index !=3) {
        
        
        if (fillDecorationLayer) {
            for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
                
                dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->setEnable(false);
                
            }
        }
        
    }else{
        if (fillDecorationLayer) {
            for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
                
                dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->setEnable(true);
                
            }
        }
    
    }
    
    
    
    AudioHelp::getInstance()->playSelectEffect();
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if(m_pCurrentSprite&&m_pIndex!=-1)
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IcePop_MenuItem[m_pIndex].c_str())->getCString()));
        kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,320 + 1000,100,Vec2(0.5, 0),kBorderNone,kBorderBottom);

        //        m_pUnitScrollbgsprite->setVisible(false);
        
        //        resetBtn->setVisible(true);
        //        m_pGameLayer->showNextButton(true);
    }
    
    if(item!=m_pCurrentSprite)
    {
        item->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_u.png",G_IcePop_MenuItem[index].c_str())->getCString()));
        
//        CHOOSE_TYPE = 1;
//        G_IcePop_MouleNum = 2;
        
        m_pCurrentSprite = item;
        m_pIndex = index;
        
        log("m_pIndex--%d",m_pIndex);
        log("index--%d",index);
        //出现
        showUnitScroll(index);
        
        
    }
    else
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IcePop_MenuItem[index].c_str())->getCString()));
        
        
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
        
        
        
    }
}
void IcePopDecorationLayer::handAction()
{
    Sprite* hand = Sprite::create("images/ice_cream/2/hand.png");
    kAdapterScreen->setExactPosition(hand, 320, 350 + 50 + 100);
    hand->setAnchorPoint(Vec2(0.5,0.5));
    hand->setTag(102);
    this->addChild(hand,50);
    
    
    MoveBy* move = MoveBy::create(1.5, Vec2(-50,-100));
    MoveBy* move1 = MoveBy::create(1.5, Vec2(50,100));
    RepeatForever* repeat = RepeatForever::create(Sequence::create(move,move1, NULL));
    
    hand->runAction(repeat);


}


void IcePopDecorationLayer::onUnitCallback(int tag,MenuItem* menuItem,int index)
{
    
    if (this->getChildByTag(102)) {
        this->getChildByTag(102)->removeFromParent();
    }
    AudioHelp::getInstance()->playEffect("add.mp3");
    log("----->m_pIndex  %d",m_pIndex);
    m_bIsTouch  = false;
 
    m_bPaintTouch = false;
    if(m_pIndex == -1){
        return;
    }
    m_pUnitScrollView->setEnable(false);
    m_iFrontIndex = -1;
    m_iBackrontIndex = -1;
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);

    if(kIAPManager->isFree(G_IcePop_MenuItem[m_pIndex].c_str(),index))
    {
        m_pUnitScrollView->setEnable(true);
        switch (m_pIndex) {
            case KIcePopCOATING:
            {
                auto cakePaint = MyPaintSprite::create(429, 500);
                cakePaint->setPosition(Vec2(200,200));
                cakePaint->setAnchorPoint(Vec2(0.5,0.5));
                
                __String* str = nullptr;
                if (G_IcePop_MouleNum == 1) {
                    
                    str = __String::createWithFormat("images/dec/category/coating/mould_1/mould1_%d.png",index);

                    kAdapterScreen->setExactPosition(cakePaint, 320, 350 + 50 + 100 + 20);
                }else if(G_IcePop_MouleNum == 2){

                    str = __String::createWithFormat("images/dec/category/coating/mould_2_3_4/mould2_3_4_%d.png",index);
                    kAdapterScreen->setExactPosition(cakePaint, 320 + 10, 350 + 50 + 100 + 20 + 20);
                    
                }else if(G_IcePop_MouleNum == 3){
                    
                    str = __String::createWithFormat("images/dec/category/coating/mould_2_3_4/mould2_3_4_%d.png",index);
                    kAdapterScreen->setExactPosition(cakePaint, 320 + 5+1 , 350 + 50 + 100 + 20 + 20 + 10 + 3);
                    
                
                }else if(G_IcePop_MouleNum == 4){
                    
                    str = __String::createWithFormat("images/dec/category/coating/mould_2_3_4/mould2_3_4_%d.png",index);
                    kAdapterScreen->setExactPosition(cakePaint, 320, 350 + 50 + 100 + 20 + 20 + 10);
                
                }else if(G_IcePop_MouleNum == 5){

                    str = __String::createWithFormat("images/dec/category/coating/mould_5/mould5_%d.png",index);
                    kAdapterScreen->setExactPosition(cakePaint, 320 - 4, 350 + 50 + 100 + 20 + 10);
                }
                cakePaint->useTarget(str->getCString());
                cakePaint->useBrush("images/button/scribble.png");
                cakePaint->setScale(0.85);
//                G_IcePop_MouleSprite->addChild(cakePaint,20);
//                kAdapterScreen->setExactPosition(cakePaint, 320, 350 + 50 + 100 + 20);
                
                decorationLayer->addChild(cakePaint);
                log("decorationLayer111111111111111111-----------");
                m_vCoasting.pushBack(cakePaint);
                
                m_bIsTouch = true;
                m_bPaintTouch=false;
                handAction();
                
                if (m_pUnitSprite) {
                    m_pUnitSprite->removeFromParent();
                }
                
                auto itemBg = Sprite::create("images/dec/itemBg.png");
                itemBg->setPosition(Vec2(-14,-8));
                itemBg->setAnchorPoint(Vec2::ZERO);
                itemBg->setTag(101);
                item->addChild(itemBg,-1);
                    
                    
                m_pUnitSprite=itemBg;
                    
               

            }
                break;
            case KIcePopSTICKS:
            {
                G_IcePop_SticksNum = index;
                __String* str = __String::createWithFormat("images/dec/category/sticks/sticks%d.png",index);
                m_pSticks->setTexture(str->getCString());
                
                ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                emitter1->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2));
                emitter1->setDuration(0.5f);
                emitter1->setAutoRemoveOnFinish(true);
                this->addChild(emitter1, 30);
                AudioHelp::getInstance()->playEffect("all star .mp3");
            }
                break;
            case KIcePopCANDY:
            {
                if (m_pUnitSprite) {
                    m_pUnitSprite->removeFromParent();
                }
                auto itemBg = Sprite::create("images/dec/itemBg.png");
                itemBg->setPosition(Vec2(-14,-8));
                itemBg->setAnchorPoint(Vec2::ZERO);
                itemBg->setTag(101);
                item->addChild(itemBg,-1);
                    
                    
                m_pUnitSprite=itemBg;
                    
            
                
                
                m_bIsTouch = true;
                m_bPaintTouch = true;
                //                m_sTouchPath = __String::createWithFormat("images/dec/category/%s/%s%d.png",G_IceCream_MenuItem[m_pIndex].c_str(),G_IceCream_MenuItem[m_pIndex].c_str(),index)->getCString();
                
                m_iFrontIndex = m_pIndex;
                m_iBackrontIndex = index;

            }
                break;
            case KIcePopSPRINKLE:
            {
                if (m_pUnitSprite) {
                    m_pUnitSprite->removeFromParent();
                }
                
                
                    
                auto itemBg = Sprite::create("images/dec/itemBg.png");
                itemBg->setPosition(Vec2(-14,-8));
                itemBg->setAnchorPoint(Vec2::ZERO);
                itemBg->setTag(101);
                item->addChild(itemBg,-1);
                    
                    
                m_pUnitSprite=itemBg;
                    
               
                
                
                m_bIsTouch = true;
                m_bPaintTouch = true;
                
                m_iFrontIndex = m_pIndex;
                m_iBackrontIndex = index;
            }
                break;
            default:
            {
                m_pGameLayer->m_pNextItemSprite->setEnabled(false);
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/category/%s/%s%d.png",G_IcePop_MenuItem[m_pIndex].c_str(),G_IcePop_MenuItem[m_pIndex].c_str(),index)->getCString());
                
                //                kAdapterScreen->setExactPosition(ice, 480, 320);
                kAdapterScreen->setExactPosition(ice, 320, 480 + 100);
                fillDecorationLayer->addChild(ice);
                ice->setScale(0);
                
                Vec2 v = ice->getPosition();
                
                auto func=CallFunc::create([&](){
                    
                    m_pGameLayer->m_pNextItemSprite->setEnabled(true);
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    kAdapterScreen->setExactPosition(emitter1, 320, 480 + 100);
                    emitter1->setDuration(0.5f);
                    emitter1->setAutoRemoveOnFinish(true);
                    this->addChild(emitter1, 30);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                });
                
                
                MoveTo* move = MoveTo::create(0.5, v);
                ScaleTo* scale = ScaleTo::create(0.5, 1);
                Spawn* spwn = Spawn::create(move,scale, NULL);
                ice->runAction(Sequence::create(MoveTo::create(0, item->getParent()->convertToWorldSpace(item->getPosition())),spwn,func,NULL));
                
            }
                break;
        }
        
    }
    else
    {
        SceneManager::getInstance()->enterShopScene();
    }
    
 
    
    
}

void IcePopDecorationLayer::onBtnCallBack(Ref* sender)
{
    
}

void IcePopDecorationLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn -> getTag())
            {
                case 51:
                {
                    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
                    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                    dialog->setContentText("Do you want to reset your food?");
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    m_pGameLayer->addToUILayer(dialog, 9999);
                }
                    
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
}

void IcePopDecorationLayer::onNegativeClick(void*)
{
    
}

void IcePopDecorationLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        
    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");
        
        decorationLayer->removeAllChildren();
        
        m_vCoasting.clear();
        
        m_pSticks->setTexture("images/ice_pop/3/stick.png");
        
        G_IcePop_SticksNum = -1;
        
        fillDecorationLayer->removeAllChildren();
        
        if(m_pUnitSprite&&m_pIndex==0){
            m_pUnitSprite ->removeFromParent();
            m_pUnitSprite=nullptr;
        }
//        m_bIsTouch = false;
//        m_bPaintTouch = false;
        
        if (this->getChildByTag(102)) {
            this->getChildByTag(102)->removeFromParent();
        }
    }
    
}
