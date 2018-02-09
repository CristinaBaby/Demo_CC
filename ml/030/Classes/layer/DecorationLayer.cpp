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
    
    CONE= 0,
    ICING,
    SPRINKLE,
    CANDY,
    FRUIT,
    BACKGROUND,

};

Color4B color[8]={Color4B(115,36,34,255),Color4B(220,147,15,255),Color4B(177,4,0,255),Color4B(83,5,5,255),Color4B(224,170,9,255),Color4B(165,112,18,255),Color4B(49,109,36,255),Color4B(132,167,10,255)};
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
    if (kIAPManager->isShowAds()) {
        
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
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

    pBgSprite = Utils::getUISprite("images/dec/decoration_bg.jpg");
    kAdapterScreen->setExactPosition(pBgSprite, 0, 640*0.5,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addToBackGroundLayer(pBgSprite, 1);

    pCone = Sprite::create("images/dec/categery/cone/cone0.png");
    kAdapterScreen->setExactPosition(pCone, 680, 20);
    this->addToContentLayer(pCone,2);
    
//    g_choose_waste = "chocolate";
    
    auto _ice = Sprite::create(__String::createWithFormat("images/dec/cream_%s.png",g_choose_waste.c_str())->getCString());
    _ice->setAnchorPoint(Vec2(0.5,0));
    _ice->setPosition(Vec2(143,570));
    pCone->addChild(_ice);
    

    

    auto ice = Sprite::create(__String::createWithFormat("images/dec/cream_%s.png",g_choose_waste.c_str())->getCString());
    ice->setAnchorPoint(Vec2(0.5,0));
    ice->setPosition(Vec2(143,570));
    pCone->addChild(ice);
    
    
    Sprite* pCategoryBgSprite = Utils::getUISprite("images/dec/banner_pad.png");
    kAdapterScreen->setExactPosition(pCategoryBgSprite, 0 - 200, 320,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addToUILayer(pCategoryBgSprite, 9);
//    pCategoryBgSprite->setVisible(false);

    m_pUnitScrollbgsprite = Utils::getUISprite("images/dec/banner_2.png");
    kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,pCategoryBgSprite->getPositionX()+ 180 + 200 - 50 ,
                                     640/2,Vec2(0, 0.5),kBorderLeft,kBorderNone);
    this->addToUILayer(m_pUnitScrollbgsprite, 5);
    m_pUnitScrollbgsprite->setVisible(false);

    m_pGirdView = KDGridView::create(Size(pCategoryBgSprite->getContentSize().width - 176,596));
    m_pGirdView->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_pGirdView->m_iCol = 1;
    //m_pGirdView->m_iRow = 4;
    m_pGirdView->m_fHSpace = 10;
    m_pGirdView->m_fVSpace = (611 - 123 *5) /6;
    m_pGirdView->m_fMarginTop = (611 - 123 * 5) /6;
    m_pGirdView->m_fMarginBottom = 110;
    kAdapterScreen->setExactPosition(m_pGirdView, 10, 640*0.5 - 700,Vec2(0,0.5),kBorderLeft,kBorderNone);
    this->addToUILayer(m_pGirdView,10);
    
    m_pAdapter = KSDecorationAdapter::create(m_vTypes);
    m_pGirdView->setAdapter(m_pAdapter);
    
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(DecorationLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);

    m_pGirdView->m_pScrollView->setContentOffset(Vec2(0,50));
    
    m_pUnitScrollView = UnitScrollView::create(Size(pCategoryBgSprite->getContentSize().width, Director::getInstance()->getWinSizeInPixels().height-100));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(DecorationLayer::onUnitCallback, this));
    kAdapterScreen->setExactPosition(m_pUnitScrollView,180,
                                     100,Vec2(0, 0),kBorderLeft,kBorderBottom);
    this->addToUILayer(m_pUnitScrollView,20);
    
 
    m_pUnitScrollView->m_iCol = 1;
    m_pUnitScrollView->m_fHSpace = 10;
    m_pUnitScrollView->m_fVSpace = 5;
    m_pUnitScrollView->m_bNeedAdjust = false;
    m_pUnitScrollView->m_fMarginBottom = 100;

   
    
    resetBtn = ui::Button::create("images/button/btn_reset@2x.png");
    resetBtn -> setAnchorPoint(Vec2(1, 1));
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    kAdapterScreen->setExactPosition(resetBtn, 10, 20 + 100,Vec2(1, 0),kBorderRight,kBorderBottom);
#else
    kAdapterScreen->setExactPosition(resetBtn, 10 + resetBtn->getContentSize().width + 10, 20,Vec2(1, 1),kBorderRight,kBorderTop);
#endif
    
    resetBtn->setTag(51);
    this->addToUILayer(resetBtn, 10);
    
    
    
    nextBtn = ui::Button::create("images/button/btn_next@2x.png");
    kAdapterScreen->setExactPosition(nextBtn, 10, 20,Vec2(1, 1),kBorderRight,kBorderTop);
    nextBtn->setTag(54);
    this->addToUILayer(nextBtn, 10);
    
    resetBtn -> addTouchEventListener(CC_CALLBACK_2(DecorationLayer::touchEvent, this));
   
    nextBtn  -> addTouchEventListener(CC_CALLBACK_2(DecorationLayer::touchEvent, this));
  
    auto func =   CallFunc::create([&](){
        
        this -> startAnimtion();
    });
    
    MoveBy* move = MoveBy::create(0.5,Vec2(200,0));
    pCategoryBgSprite->runAction(Sequence::create(DelayTime::create(0.8),move,func, NULL));
    
    auto funcSounds=CallFunc::create([&](){
        
        AudioHelp::getInstance()->playEffect("add.mp3");
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.8),funcSounds, NULL));
    

    
    return true;
}

void DecorationLayer::startAnimtion()
{
   
    auto funcSounds=CallFunc::create([&](){
        
         AudioHelp::getInstance()->playEffect("banner drop.mp3");
         showBread();
        
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,-390), 0.5);
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(1.0),funcSounds, NULL));
    MoveBy* move = MoveBy::create(0.5,Vec2(0,700));
    m_pGirdView->runAction(Sequence::create(DelayTime::create(0.5),move,NULL));
}
 void DecorationLayer::showBread()
{
  
   
}

bool  DecorationLayer::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<DEC_ITEM_NUMS; i++) {
        __String* str = __String::createWithFormat("images/dec/btn_%s@2x.png",DEC_ITEM[i].c_str());
        
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
    
//    m_pGirdView->setEnable(false);
    m_bIsTouch = false ;
    AudioHelp::getInstance()->playSelectEffect();
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if(m_pCurrentSprite&&m_pIndex!=-1)
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/btn_%s@2x.png",
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));

        m_pUnitScrollView->setVisible(false);
        m_bIsShowUnit = false;
        
        
    }
    
    if(item!=m_pCurrentSprite)
    {
    item->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/btn_%s_h@2x.png",
                                                                   DEC_ITEM[index].c_str())->getCString()));
   
        m_pUnitScrollView->loadingUnitScrollViewWithType(index);
        m_pUnitScrollView->setVisible(true);
        m_pCurrentSprite = item;
        m_pIndex = index;
    }
    else
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("images/dec/btn_%s@2x.png",
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));
        m_bIsShowUnit = false;
        m_pUnitScrollView->setVisible(false);
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
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
    if(kIAPManager->isFree(DEC_ITEM[m_pIndex].c_str(),index))
    {
        switch (m_pIndex) {
            case  CONE:
            {
                m_pUnitScrollView->setEnable(false);
                m_pGirdView->setEnable(false);
                
                auto cone = Sprite::create(__String::createWithFormat("images/dec/categery/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                cone->setPosition(item->convertToWorldSpace(Vec2::ZERO));
                this->addToContentLayer(cone, 5);
                cone->setScale(0);
                
                g_dec_cone = index;
                
                auto func = CallFunc::create([=](){
                
                    pCone->setTexture(__String::createWithFormat("images/dec/categery/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                    m_pUnitScrollView->setEnable(true);
                    m_pGirdView->setEnable(true);
                    
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                    
                    emitter1->setPosition(decorationLayer->convertToWorldSpace(pCone->getPosition()) + Vec2(0,200));
                    
                    emitter1->setAutoRemoveOnFinish(true);
                    this->addChild(emitter1, 30);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                    
                });
                ScaleTo* scale = ScaleTo::create(0.5,1.0);

                

                MoveTo* move = MoveTo::create(0.5, decorationLayer->convertToWorldSpace(pCone->getPosition()));
                Spawn* sp = Spawn::create(scale,move, NULL);
                cone->runAction(Sequence::create(sp,RemoveSelf::create(),func,NULL));
                
                
            }
                break;
            case  ICING:
            {
                m_pUnitScrollView->setEnable(false);
                m_pGirdView->setEnable(false);
                
                auto icing = Sprite::create(__String::createWithFormat("images/dec/categery/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                icing->setPosition(item->convertToWorldSpace(Vec2::ZERO));
                this->addToContentLayer(icing, 5);
                icing->setScale(0);
                
                auto func = CallFunc::create([=](){
                    
                    if (decorationLayer->getChildByTag(101)) {
                        decorationLayer->getChildByTag(101)->removeFromParent();
                    }
                    
                    auto icing = Sprite::create(__String::createWithFormat("images/dec/categery/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                    icing->setPosition(pCone->getPosition() + Vec2(-5,294 + 114));
                    decorationLayer->addChild(icing);
                    icing->setTag(101);
                    
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("particle/decorateParticle.plist");
                  
                    emitter1->setPosition(decorationLayer->convertToWorldSpace(icing->getPosition()));
                
                    emitter1->setAutoRemoveOnFinish(true);
                    this->addChild(emitter1, 30);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
                    
                    
                    
                    m_pUnitScrollView->setEnable(true);
                    m_pGirdView->setEnable(true);
                });
                
                
                ScaleTo* scale = ScaleTo::create(0.5,1.0);
                MoveTo* move = MoveTo::create(0.5, decorationLayer->convertToWorldSpace(pCone->getPosition() + Vec2(0,294+113)));
                Spawn* sp = Spawn::create(scale,move, NULL);
                icing->runAction(Sequence::create(sp,func,RemoveSelf::create(),NULL));
                
            }
                break;
            case  SPRINKLE:
            case  CANDY:
            case  FRUIT:
            {
                m_pGirdView->setEnable(false);
                m_pUnitScrollView->setEnable(false);
                
                int v_rand = rand()%4 * 45;
                
                auto ice = FillMaterialModel::create(__String::createWithFormat("images/dec/categery/%s/%s%d.png",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                kAdapterScreen->setExactPosition(ice, 580 + v_rand, 450);
                decorationLayer->addChild(ice,10);
                ice->setUserData((void*)1);
                ice->setScale(0);
                
                Rect rect=Rect(Director::getInstance()->getVisibleOrigin().x + 400,Director::getInstance()->getVisibleOrigin().x,Director::getInstance()->getVisibleSize().width - 400,Director::getInstance()->getVisibleSize().height);
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
            case  BACKGROUND:
            {
                g_dec_bg = index;
                pBgSprite->setTexture(__String::createWithFormat("images/dec/categery/%s/%s%d.jpg",DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                
                
            }
                break;
                
            default:
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
                    m_bIsShowReturn=true;
                    Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
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
                    //下一步
                    for (int i = 0; i<decorationLayer->getChildren().size(); i++) {
                        
                        int data = (uintptr_t)decorationLayer->getChildren().at(i)->getUserData();
                        if (data == 1) {
                            
                            dynamic_cast<FillMaterialModel*>(decorationLayer->getChildren().at(i))->stopTipsFrameStatus();
                            dynamic_cast<FillMaterialModel*>(decorationLayer->getChildren().at(i))->setNotDeleteSelf();
                        }
                    }
                    
                    for (int i = 0; i<notEatLayer->getChildren().size(); i++) {
                        
                        int data = (uintptr_t)notEatLayer->getChildren().at(i)->getUserData();
                        if (data == 1) {
                            
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->stopTipsFrameStatus();
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->setNotDeleteSelf();
                        }
                    }
                    
                    
                    Scene* scene = nullptr;
                    
                    SceneChangeManager->enterEatScene();
                    if(kIAPManager->isShowAds())
                    {
                        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
                       

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
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
        Director::getInstance()->popToRootScene();
        SceneChangeManager->enterHomeScene();

    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");

        
       
        g_dec_bg = -1;
        g_dec_cone = -1;
        pCone->setTexture("images/dec/categery/cone/cone0.png");
        pBgSprite->setTexture("images/dec/decoration_bg.jpg");
        
        decorationLayer->removeAllChildren();
        
        
       
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
            Dialog* dialog= Dialog::create(Size(440,300), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
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





