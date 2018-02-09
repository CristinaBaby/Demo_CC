//
//  OreoDecorationLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-3.
//
//

#include "DecorationLayer.h"
#include "IAPManager.h"
#include "AdapterScreen.h"
#include "KSDecorationAdapter.h"
#include "Global.h"
#include "ToolSprite.h"
#include "ScribbleTouchNode.h"
#include "AudioHelp.h"
#include "Global.h"
#include "MyPaintSprite.h"
#include "FillMaterialModel.h"

#include "EatScene.h"


enum DecorateType
{
    
    PLATES= 0,
    CAKE,
    ICINGS,
    TOPPERS,
    CANDY,
    EXTRAS,

};

Color4B color[8]={Color4B(115,36,34,255),Color4B(220,147,15,255),Color4B(177,4,0,255),Color4B(83,5,5,255),Color4B(224,170,9,255),Color4B(165,112,18,255),Color4B(49,109,36,255),Color4B(132,167,10,255)};

int topperFire[15]={1,3,4,5,6,18,19,20,21,22,23,24,25,26,27};

//Vec2 firePos[15]={Vec2(28,123),Vec2(29,128),Vec2(15,141),Vec2(13,135),Vec2(9,139),Vec2(12,154),Vec2(11,155),Vec2(29,146),Vec2(9,151),Vec2(17,158),Vec2(11,154),Vec2(14,158),Vec2(11,145),Vec2(14,142),Vec2(12,144)};

DecorationLayer::DecorationLayer():m_bIsFirst(true),m_bIsShowUnit(false)
{
   
    decorationLayer = CCLayer::create();
    decorationLayer->retain();
    
    notEatLayer= CCLayer::create();
    notEatLayer->retain();
    

}
DecorationLayer::~DecorationLayer()
{
    log("decorationLayer-----%d",decorationLayer->getReferenceCount());
    decorationLayer->release();
    decorationLayer = NULL;
    
    notEatLayer->release();
    notEatLayer = NULL;
    log("······················~DecorationLayer()");
}

DecorationLayer* DecorationLayer::create()
{
    DecorationLayer* dec = new DecorationLayer();
    
    if(dec&&dec->init())
    {
        dec->autorelease();
        return dec;
    }
    CC_SAFE_DELETE(dec);
    return nullptr;
}


void DecorationLayer::onEnter()
{
    BaseLayer::onEnter();
    m_pUnitScrollView->setEnable(true);
    
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(54));
    btn->setTouchEnabled(true);
    
    if(m_pIndex!=-1)
    {
       
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
        
    }
    
    if(!notEatLayer->getParent()){
    
        this->addToContentLayer(notEatLayer, 5);
        for (int i = 0; i<notEatLayer->getChildren().size(); i++) {
            
            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->setEnable(true);
        }
    }
    
}
void DecorationLayer::onExit()
{
    BaseLayer::onExit();

}
bool DecorationLayer::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        m_pIndex=-1;
        m_bIsTouch = false;
       
        isInit=true;
   
        
    }while(0);
    
    return isInit;

}
bool DecorationLayer::initUI()
{
    this->addToContentLayer(decorationLayer, 5);
    this->addToContentLayer(notEatLayer, 5);
    
    
    pBgSprite = Utils::getUISprite("images/dec/decoration_bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 320,480);
    this->addToBackGroundLayer(pBgSprite, 1);

//    
    
    m_pPlate = Sprite::create("images/cream/stands.png");
    m_pPlate->setPosition(Vec2(pBgSprite->getContentSize().width/2,460));
    pBgSprite->addChild(m_pPlate,10);
    
    auto cake = Sprite::create(__String::create("images/cream/cream.png")->getCString());
    cake->setPosition(Vec2(285,410));
    m_pPlate->addChild(cake);
    cake->setTag(CAKE);
    
    Sprite* pCategoryBgSprite = Utils::getUISprite("images/dec/dec_bar_btn.png");
    kAdapterScreen->setExactPosition(pCategoryBgSprite, 320, 110,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToUILayer(pCategoryBgSprite, 9);
//    pCategoryBgSprite->setVisible(false);

    m_pUnitScrollbgsprite = Utils::getUISprite("images/dec/dec_bar.png");
//    kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,180 + 200 - 50 ,
//                                     640/2,Vec2(0, 0.5),kBorderLeft,kBorderNone);
    this->addToUILayer(m_pUnitScrollbgsprite, 5);
    kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,320,
                                     10,Vec2(0.5, 1),kBorderNone,kBorderTop);
    m_pUnitScrollbgsprite->setVisible(false);

    m_pGirdView = KDGridView::create(Size(168*3,124));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
//    m_pGirdView->m_iCol = 1;
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 10;
//    m_pGirdView->m_fVSpace = (611 - 123 *5) /6;
//    m_pGirdView->m_fMarginTop = (611 - 123 * 5) /6;
    kAdapterScreen->setExactPosition(m_pGirdView, 320, 110,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToUILayer(m_pGirdView,10);
    
    m_pAdapter = KSDecorationAdapter::create(m_vTypes);
    m_pGirdView->setAdapter(m_pAdapter);
    
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(DecorationLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);

    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-500,0));
    
    m_pUnitScrollView = UnitScrollView::create(Size(577, 110));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(DecorationLayer::onUnitCallback, this));
//    kAdapterScreen->setExactPosition(m_pUnitScrollView,320,
//                                     80+105,Vec2(0.5, 0),kBorderNone,kBorderBottom);
//    this->addToUILayer(m_pUnitScrollView,20);
    m_pUnitScrollView->setAnchorPoint(Vec2::ZERO);
    m_pUnitScrollView->setPosition(Vec2(20,20));
    m_pUnitScrollbgsprite->addChild(m_pUnitScrollView);
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
 
    m_pUnitScrollView->m_iRow = 1;
    m_pUnitScrollView->m_fHSpace = 10;
   
//    m_pUnitScrollView->m_fMarginLeft=30;
    m_pUnitScrollView->m_fMarginTop = 0;
    m_pUnitScrollView->m_bNeedAdjust = true;
    m_pUnitScrollView->m_pScrollView->setBounceable(true);
   
    
    resetBtn = ui::Button::create("images/button/btn_reset@2x.png");
    resetBtn -> setAnchorPoint(Vec2(1, 1));
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    kAdapterScreen->setExactPosition(resetBtn, 5, 110 + 132,Vec2(0, 0),kBorderLeft,kBorderBottom);
#else
    kAdapterScreen->setExactPosition(resetBtn, 5, 110 + 132,Vec2(0, 0),kBorderLeft,kBorderBottom);
#endif
    
    resetBtn->setTag(51);
    this->addToUILayer(resetBtn, 10);
    
    
    
    nextBtn = ui::Button::create("images/button/btn_next@2x.png");
    kAdapterScreen->setExactPosition(nextBtn, 5, 110 + 132,Vec2(1, 0),kBorderRight,kBorderBottom);
    nextBtn->setTag(54);
    this->addToUILayer(nextBtn, 10);
    
    resetBtn -> addTouchEventListener(CC_CALLBACK_2(DecorationLayer::touchEvent, this));
   
    nextBtn  -> addTouchEventListener(CC_CALLBACK_2(DecorationLayer::touchEvent, this));
  
    auto func =   CallFunc::create([&](){
        
        this -> startAnimtion();
    });
    
//    MoveBy* move = MoveBy::create(0.5,Vec2(200,0));
//    pCategoryBgSprite->runAction(Sequence::create(DelayTime::create(0.8),move,func, NULL));
//    
//    auto funcSounds=CallFunc::create([&](){
//        
//        AudioHelp::getInstance()->playEffect("add.mp3");
//        
//    });
//    
    this->runAction(Sequence::create(DelayTime::create(0.8),func, NULL));
    

    
    return true;
}

void DecorationLayer::startAnimtion()
{
   
    auto funcSounds=CallFunc::create([&](){
        
        
         showBread();
         AudioHelp::getInstance()->playEffect("banner drop.mp3");
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 0.5);
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.25),funcSounds, NULL));

//    AudioHelp::getInstance()->playEffect("banner drop.mp3");

}
 void DecorationLayer::showBread()
{
  
   
}

bool  DecorationLayer::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<DEC_ITEM_NUMS; i++) {
        __String* str = __String::createWithFormat("images/dec/type_%s0.png",DEC_ITEM[i].c_str());

        log("%s",str->getCString());
        
        Sprite* data = Sprite::create(str->getCString());
        m_vTypes.pushBack(data);
    }

    return true;
}

void DecorationLayer::onTypeCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    log("button----->%d",tag);

}

void DecorationLayer::onTypePageCallback(Ref* sender, Control::EventType controlEvent)
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

void DecorationLayer::_onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    
}
void DecorationLayer::scrollViewTouchBegin(Vec2 worldPoint)
{
    
}
void DecorationLayer::scrollViewWillScroll()
{
    
}
//GirdView点击回调
void DecorationLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    m_bIsTouch = false ;
    AudioHelp::getInstance()->playSelectEffect();
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if(m_pCurrentSprite&&m_pIndex!=-1)
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/type_%s0.png",
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));

        m_pUnitScrollView->setVisible(false);
        m_pUnitScrollbgsprite->setVisible(false);
        m_bIsShowUnit = false;
        
        
    }
    
    if(item!=m_pCurrentSprite)
    {
        item->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/type_%s1.png",
                                                                   DEC_ITEM[index].c_str())->getCString()));
   
        m_pUnitScrollView->loadingUnitScrollViewWithType(index);
        m_pUnitScrollView->setVisible(true);
        m_pUnitScrollbgsprite->setVisible(true);
        m_pCurrentSprite = item;
        m_pIndex = index;
    }
    else
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/type_%s0.png",
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));
        m_bIsShowUnit = false;
        m_pUnitScrollView->setVisible(false);
        m_pUnitScrollbgsprite->setVisible(false);
        m_pGirdView->setEnable(true);
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
        
        
    }
}

void DecorationLayer::showHand()
{
    auto hand = Sprite::create("images/makeDog/hand.png");
    kAdapterScreen->setExactPosition(hand,480, 200);
    this->addToContentLayer(hand,20);
    
    MoveBy* move = MoveBy::create(0.5,Vec2(50,50));
    MoveBy* move1 = MoveBy::create(0.5,Vec2(50,-50));
    MoveBy* move2 = MoveBy::create(0.5,Vec2(50,50));
    MoveBy* move3 = MoveBy::create(0.5,Vec2(50,-50));
    
    hand->runAction(Sequence::create(move,move1, move2,move3,RemoveSelf::create(),NULL));

}
void DecorationLayer::onUnitCallback(int tag,MenuItem* menuItem,int index)
{
    AudioHelp::getInstance()->playEffect("add.mp3");
    log("----->m_pIndex  %d",m_pIndex);
    if(m_pIndex==-1){
        return;
        
    }
    
    if(m_pCurrentSprite){
        
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/type_%s0.png",
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));
        m_pCurrentSprite=nullptr;
    }
    
//    m_pUnitScrollView->setVisible(false);
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
    if(kIAPManager->isFree(DEC_ITEM[m_pIndex].c_str(),index))
    {
        switch (m_pIndex) {
            case TOPPERS:{
            
            }
                break;
            default:
            {
                m_pGirdView->setEnable(false);
                m_pUnitScrollView->setEnable(false);
                
                int v_rand = rand()%4 * 45;
                
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                kAdapterScreen->setExactPosition(ice, 280 + v_rand, 480);
                decorationLayer->addChild(ice,10);
                ice->setUserData((void*)1);
                ice->setScale(0);
                
                Rect rect=Rect(Director::getInstance()->getVisibleOrigin().x,Director::getInstance()->getVisibleOrigin().y + 100,Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height);
                ice->setMoveRect(rect);
                
                Vec2 v = ice->getPosition();
                auto func=CallFunc::create([=](){
                    
                    m_pGirdView->setEnable(true);
                    m_pUnitScrollView->setEnable(true);
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    kAdapterScreen->setExactPosition(emitter1, 580 + v_rand, 450);
                    
                    emitter1->setPosition(v);
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
        
        
    }else{
    
        m_pUnitScrollView->setEnable(false);
        SceneManager::getInstance()->enterShopScene();
    }

}

void DecorationLayer::onBtnCallBack(Ref* sender)
{
    
}
int DecorationLayer::checkTopper(int index)
{
    for (int i = 0; i<15; i++) {
        
        if (index == topperFire[i]) {
            return i;
        }
        
        
    }
    return -1;

}
void DecorationLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
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
                    m_bIsShowReturn = true;
                    Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
                    kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
                    dialog->setContentText("Do you want to reset your food?");
                    dialog->setPositiveBtnText("");
                    dialog->setNegativeBtnText("");
                    dialog->setCallback(this);
                    this->addToUILayer(dialog, 9999);
                }
                    
                    break;
               
                case 54:
                {
                    btn->setTouchEnabled(false);
                    
                    
                    auto func = CallFunc::create([=](){
                    
                        btn->setTouchEnabled(true);
                    
                    });
                    
                    ActionUtils::delayTimeAction(this,0.5,func);
                    
                    
                    //下一步
                    for (int i = 0; i<decorationLayer->getChildren().size(); i++) {
                        
                        size_t data = (size_t)decorationLayer->getChildren().at(i)->getUserData();
                        if (data == 1) {
                            
                            dynamic_cast<FillMaterialModel*>(decorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                            dynamic_cast<FillMaterialModel*>(decorationLayer->getChildren().at(i))->setNotDeleteSelf();
                        }
                    }
                    
                    for (int i = 0; i<notEatLayer->getChildren().size(); i++) {
                        
                        size_t data = (size_t)notEatLayer->getChildren().at(i)->getUserData();
                        if (data == 2) {
                            
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->stopTipsFrameStatus();
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->setNotDeleteSelf();
                        }
                    }
                    
                    
//                    SceneChangeManager->enterEatScene();
                    
                    if(kIAPManager->isShowAds())
                    {
                        

                        
                        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
                        AdLoadingLayerBase::loadingDoneCallback = []()
                        {
                            
                            SceneChangeManager->enterEatScene();
                        };
                      

                    }
                    else
                    {
                        SceneChangeManager->enterEatScene();
                    }
                }
                    
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
}

void DecorationLayer::onNegativeClick(void*)
{
    m_bIsShowReturn = false;
}

void DecorationLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        if(kIAPManager->isShowAds())
        {
            
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            AdLoadingLayerBase::loadingDoneCallback = []()
            {
                Director::getInstance()->popToRootScene();
                SceneChangeManager->enterHomeScene();
            };
        }
        else
        {
            Director::getInstance()->popToRootScene();
            SceneChangeManager->enterHomeScene();
        }
        
    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");

        
       
        g_dec_bg = -1;
        g_dec_plates = -1;
        g_eat_cake = false;
        m_pPlate->getChildByTag(CAKE)->setVisible(true);
        m_pPlate->setTexture("images/cream/stands.png");
        decorationLayer->removeAllChildren();
        
        notEatLayer->removeAllChildren();
       
    }
    
}
// 监听Android返回键事件
void DecorationLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText(__String::createWithFormat("Are you sure you want to start over?")->getCString());
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }
}



