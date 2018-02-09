//
//  IceCreamIceCreamDecorationLayer.cpp
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/10.
//
//

#include "IceCreamDecorationLayer.h"
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
#include "ActionUtils.h"

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
IceCreamDecorationLayer::IceCreamDecorationLayer(GameLayer* gameLayer)
{
    m_pGameLayer = gameLayer;
    decorationLayer = CCLayer::create();
    decorationLayer->retain();
    
    iceDecorationLayer = CCLayer::create();
    iceDecorationLayer->retain();
    
    fillDecorationLayer = CCLayer::create();
    fillDecorationLayer->retain();
}
IceCreamDecorationLayer::~IceCreamDecorationLayer()
{
    log("~IceCreamDecorationLayer()");
    fillDecorationLayer->release();
    fillDecorationLayer = NULL;
    
    log("G_IceCream_CupSprite--%d",G_IceCream_CupSprite->getReferenceCount());
    
    CC_SAFE_RELEASE_NULL(decorationLayer);
    CC_SAFE_RELEASE_NULL(iceDecorationLayer);
    CC_SAFE_RELEASE_NULL(G_IceCream_CupSprite);
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "purchase_Finish");
    
}
IceCreamDecorationLayer* IceCreamDecorationLayer::create(GameLayer* gameLayer)
{
    IceCreamDecorationLayer* dec = new IceCreamDecorationLayer(gameLayer);
    
    
    
    if(dec&&dec->init())
    {
        dec->autorelease();
        
        return dec;
    }
    CC_SAFE_DELETE(dec);
    return nullptr;
}


void IceCreamDecorationLayer::onEnter()
{
    BaseLayer::onEnter();
    m_pGameLayer->isDecorateEnd = true;
    m_pGameLayer->showNextButton(true);
m_pUnitScrollView->setEnable(true);
    kAdapterScreen->setExactPosition(m_pGameLayer->m_pNextItemSprite, 10, 10 + 100 + 150,Vec2(1, 0),kBorderRight,kBorderBottom);
    
    if(G_IceCream_CupSprite){
        G_IceCream_CupSprite->removeFromParent();
        kAdapterScreen->setExactPosition(G_IceCream_CupSprite, 320, 220,Vec2(0.5,0),kBorderNone,kBorderBottom);
        this->addChild(G_IceCream_CupSprite,3);
        G_IceCream_CupSprite->setScale(1);
        
        for(int i= 0;i < G_IceCream_CupSprite->getChildren().size();i++){
            
            dynamic_cast<FillMaterialModel*>(G_IceCream_CupSprite->getChildren().at(i))->setEnable(false);
            dynamic_cast<FillMaterialModel*>(G_IceCream_CupSprite->getChildren().at(i))->setNotDeleteSelf();
            
        }
    
    }
    
   
        
    auto cup = Sprite::create(__String::createWithFormat("images/ice_cream/1/cup_%d.png",G_IceCream_CupNum)->getCString());
    kAdapterScreen->setExactPosition(cup, 320, 220 ,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addChild(cup,9);
        
   
   
    
}
void IceCreamDecorationLayer::onExit()
{
    BaseLayer::onExit();
    
    for (int i = 0; i<fillDecorationLayer->getChildren().size(); i++) {
        
        dynamic_cast<FillMaterialModel*>(fillDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
        
    }
    for (int i = 0; i<iceDecorationLayer->getChildren().size(); i++) {
        
        dynamic_cast<FillMaterialModel*>(iceDecorationLayer->getChildren().at(i))->stopTipsFrameStatus();
        
    }
}
void IceCreamDecorationLayer::showBanner(Ref* r)
{

    log("bannerDidShow11111");
}
void IceCreamDecorationLayer::hideBanner(Ref* r)
{

}
bool IceCreamDecorationLayer::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        m_pIndex=-1;
        m_bIsTouch = true;
        m_bIsScrollBgShow = false;
        m_pUnitSprite = nullptr;
        
        
        m_iFrontIndex = -1;
        m_iBackrontIndex = -1;
        isInit=true;
        
        
    }while(0);
    
    return isInit;
    
}
bool IceCreamDecorationLayer::initUI()
{
    this->addToContentLayer(decorationLayer, 5);
    
    this->addChild(iceDecorationLayer, 12);
    
    this->addToContentLayer(fillDecorationLayer, 6);
    
    pBgSprite = Utils::getUISprite("images/ice_cream/decoration/bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 320, 480);
    this->addToBackGroundLayer(pBgSprite, 1);

    auto pBg = Utils::getUISprite("images/ice_cream/decoration/bg1.png");
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
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(IceCreamDecorationLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    //m_pGirdView->m_fMarginBottom = 200;
    
    
    
    m_pUnitScrollView = UnitScrollView::create(Size(607, 180));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(IceCreamDecorationLayer::onUnitCallback, this));
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pUnitScrollView->setAnchorPoint(Vec2(0.5,0.5));
    m_pUnitScrollView->setPosition(Vec2(m_pUnitScrollbgsprite->getContentSize().width/2,m_pUnitScrollbgsprite->getContentSize().height/2 ));
    m_pUnitScrollbgsprite->addChild(m_pUnitScrollView);
    
    m_pUnitScrollView->setZOrder(8);
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
    resetBtn -> addTouchEventListener(CC_CALLBACK_2(IceCreamDecorationLayer::touchEvent, this));
  
//    m_pGameLayer->m_pNextItemSprite->setPositionY(m_pGameLayer->m_pNextItemSprite->getPositionY()+150);
    
    m_pGameLayer->showNextButton(true);
    
//    m_pUnitScrollbgsprite->setPositionX(m_pUnitScrollbgsprite->getPositionX()+1000);
    m_pGirdView->setPositionX(m_pGirdView->getPositionX()+1000);
    
    startAnimtion();
   
    
    EventListenerTouchOneByOne* e = EventListenerTouchOneByOne::create();
    e->onTouchBegan = CC_CALLBACK_2(IceCreamDecorationLayer::TouchBegan, this);
    e->onTouchEnded = CC_CALLBACK_2(IceCreamDecorationLayer::TouchEnded, this);
    e->onTouchMoved = CC_CALLBACK_2(IceCreamDecorationLayer::TouchMoved, this);
    e->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(IceCreamDecorationLayer::purchaseFinish), "purchase_Finish", NULL);
    return true;
}
bool IceCreamDecorationLayer::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!m_bIsTouch) {
        return false;
    }
    
    if(m_iFrontIndex != -1){
        
        auto sprite = Sprite::create(__String::createWithFormat("images/dec/category/%s/%s%d.png",G_IceCream_MenuItem[m_iFrontIndex].c_str(),G_IceCream_MenuItem[m_iFrontIndex].c_str(),m_iBackrontIndex)->getCString());
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
void IceCreamDecorationLayer::TouchEnded(Touch *pTouch, Event *pEvent)
{
    
}
void IceCreamDecorationLayer::TouchMoved(Touch *pTouch, Event *pEvent)
{
    log("move  x:%f  y:%f",pTouch->getLocation().x,pTouch->getLocation().y);
    
    
    
}void IceCreamDecorationLayer::purchaseFinish(Ref*)
{
    if (m_pIndex != -1) {
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
    }
    
    
}
void IceCreamDecorationLayer::startAnimtion()
{
    
    auto funcSounds=CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("banner drop.mp3");
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5),funcSounds, NULL));
    
    MoveBy* move = MoveBy::create(1.0,Vec2(-1000,0));
    m_pGirdView->runAction(Sequence::create(DelayTime::create(0.5),move,NULL));
    
    
    
}
bool  IceCreamDecorationLayer::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<6; i++) {
        __String* str = __String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IceCream_MenuItem[i].c_str());
        Sprite* data = Sprite::create(str->getCString());
        m_vTypes.pushBack(data);
    }

    return true;
}

void IceCreamDecorationLayer::onTypeCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    log("button----->%d",tag);
    
}

void IceCreamDecorationLayer::onTypePageCallback(Ref* sender, Control::EventType controlEvent)
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

void IceCreamDecorationLayer::_onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    
}
void IceCreamDecorationLayer::scrollViewTouchBegin(Vec2 worldPoint)
{
}
void IceCreamDecorationLayer::scrollViewWillScroll()
{
}
void IceCreamDecorationLayer::hideUnitScroll()
{
    m_pUnitScrollView->setVisible(false);
    auto func1=CallFunc::create([&](){
//        resetBtn->setVisible(true);
//        m_pGameLayer->showNextButton(true);
        
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
        
        m_bIsTouch = true;
        
        m_bIsScrollBgShow = false;
        m_pGirdView->setScrollEnable(true);
    });
    
//    MoveBy* move = MoveBy::create(0.5, Vec2(1000,0));
//    m_pUnitScrollbgsprite->runAction(Sequence::create(move,func1,NULL));

//    ActionUtils::dropOut(m_pUnitScrollbgsprite, Vec2(1000,0),1.0,func1);

}
void IceCreamDecorationLayer::showUnitScroll(int index)
{

    m_pUnitScrollView->loadingUnitScrollViewWithType(index);
    m_pUnitScrollView->setVisible(false);
    auto func=CallFunc::create([&](){
        
        log("===%d",index);
       
        m_pUnitScrollView->setVisible(true);
        //            m_pUnitScrollbgsprite->setVisible(true);
        
//        resetBtn->setVisible(false);
//        m_pGameLayer->showNextButton(false);
//
        m_bIsTouch = true;
        
        m_bIsScrollBgShow = true;
        m_pGirdView->setScrollEnable(true);
    });
    
    MoveBy* move = MoveBy::create(0.05, Vec2(-1000,0));
    m_pUnitScrollbgsprite->runAction(Sequence::create(move,func,NULL));
    


}

//GirdView点击回调
void IceCreamDecorationLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    if(m_pUnitSprite){
        m_pUnitSprite ->removeFromParent();
        m_pUnitSprite=nullptr;
    }
    
    if (index!=3 && index!=4&&index!=0 && index!=5) {
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
    
    
    m_bIsTouch = false;
    
    m_iFrontIndex = -1;
    m_iBackrontIndex = -1;
    
    AudioHelp::getInstance()->playSelectEffect();
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if(m_pCurrentSprite&&m_pIndex!=-1)
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IceCream_MenuItem[m_pIndex].c_str())->getCString()));
//        m_pUnitScrollView->setVisible(false);
        
//        m_pUnitScrollbgsprite->setPositionX(m_pUnitScrollbgsprite->getPositionX() + 1000);

        kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,320 + 1000,100,Vec2(0.5, 0),kBorderNone,kBorderBottom);
    }
    
    if(item!=m_pCurrentSprite)
    {
        item->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_u.png",G_IceCream_MenuItem[index].c_str())->getCString()));
        
//        CHOOSE_TYPE = 0;
        
        m_pCurrentSprite = item;
        m_pIndex = index;
        
        log("m_pIndex--%d",m_pIndex);
        log("index--%d",index);
        //出现
        showUnitScroll(index);
        
        

    }
    else
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IceCream_MenuItem[index].c_str())->getCString()));
        
        
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
//        m_pGirdView->setScrollEnable(true);
    }
}



void IceCreamDecorationLayer::onUnitCallback(int tag,MenuItem* menuItem,int index)
{
    m_iFrontIndex = -1;
    m_iBackrontIndex = -1;
    AudioHelp::getInstance()->playEffect("add.mp3");
    
    if(m_pIndex == -1){
        return;
    }
    m_pUnitScrollView->setEnable(false);
    log("----->m_pIndex  %d",m_pIndex);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
//    m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/dec_icon_%s_d.png",G_IceCream_MenuItem[m_pIndex].c_str())->getCString()));
    if(kIAPManager->isFree(G_IceCream_MenuItem[m_pIndex].c_str(),index))
    {
         m_pUnitScrollView->setEnable(true);
        switch (m_pIndex) {
            case KCANDY:
            case KSPRINKLE:
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
            
                m_iFrontIndex = m_pIndex;
                m_iBackrontIndex = index;
            }
                break;
            case KEXTRAS:
            {
                
                m_pGameLayer->m_pNextItemSprite->setEnabled(false);
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/category/%s/%s%d.png",G_IceCream_MenuItem[m_pIndex].c_str(),G_IceCream_MenuItem[m_pIndex].c_str(),index)->getCString());
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
            default:
            {
                m_bIsTouch  = false;
                
                m_pGameLayer->m_pNextItemSprite->setEnabled(false);
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/category/%s/%s%d.png",G_IceCream_MenuItem[m_pIndex].c_str(),G_IceCream_MenuItem[m_pIndex].c_str(),index)->getCString());
                kAdapterScreen->setExactPosition(ice, 320, 480  + 100);
                fillDecorationLayer->addChild(ice);
                ice->setScale(0);
                
                Vec2 v = ice->getPosition();
                auto func=CallFunc::create([&](){
                    m_pGameLayer->m_pNextItemSprite->setEnabled(true);
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    kAdapterScreen->setExactPosition(emitter1, 320, 480  + 100);
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

//    
//    if (m_bIsScrollBgShow) {
//        
//        hideUnitScroll();
//    }
    

}

void IceCreamDecorationLayer::onBtnCallBack(Ref* sender)
{
    
}

void IceCreamDecorationLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
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

void IceCreamDecorationLayer::onNegativeClick(void*)
{
    
}

void IceCreamDecorationLayer::onPositiveClick(void* type)
{
    
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        
    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");
        
       decorationLayer->removeAllChildren();
        
        iceDecorationLayer->removeAllChildren();
        
        fillDecorationLayer->removeAllChildren();
        
//        if(m_pUnitSprite){
//            m_pUnitSprite ->removeFromParent();
//            m_pUnitSprite=nullptr;
//        }
    }
    
}
