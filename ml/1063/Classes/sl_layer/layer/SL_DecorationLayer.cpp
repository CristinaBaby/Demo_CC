//
//  OreoSL_DecorationLayer.cpp
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-3.
//
//

#include "SL_DecorationLayer.h"
#include "IAPManager.h"
#include "AdapterScreen.h"
#include "KSDecorationAdapter.h"
#include "Global.h"
#include "ToolSprite.h"
//#include "ScribbleTouchNode.h"
#include "AudioHelp.h"
#include "Global.h"
#include "MyPaintSprite.h"
#include "FillMaterialModel.h"
#include "WeddingFoodData.h"
#include "EatScene.h"
#include "EatLayer.h"
#include "MainScene.h"
#include "LQ_adLoadingLayer.h"
#include "ShopLayer.h"
#include "ActionHelper.h"
#include "BezierCreator.h"
#include "HomeLayer.h"
// const string SL_DecorationLayer::UIPATH = "images/dec/";
REGIST_SECENE(SL_DecorationLayer);
enum DecorateType
{
    CAKE = 0,
    ICINGS,
    TOPPERS,
    CANDY,  
    PLATES,
};

Color4B color[8]={Color4B(115,36,34,255),Color4B(220,147,15,255),Color4B(177,4,0,255),Color4B(83,5,5,255),Color4B(224,170,9,255),Color4B(165,112,18,255),Color4B(49,109,36,255),Color4B(132,167,10,255)};
SL_DecorationLayer::SL_DecorationLayer():m_bIsFirst(true),m_bIsShowUnit(false),m_pTapTitle(nullptr)
{
   
    decorationLayer = CCLayer::create();
    decorationLayer->retain();
    
    notEatLayer= CCLayer::create();
    notEatLayer->retain();
    
    

}
SL_DecorationLayer::~SL_DecorationLayer()
{
    log("SL_DecorationLayer-----%d",decorationLayer->getReferenceCount());
    decorationLayer->release();
    decorationLayer = NULL;
    
    notEatLayer->release();
    notEatLayer = NULL;
    log("······················~SL_DecorationLayer()");
}

SL_DecorationLayer* SL_DecorationLayer::create()
{
    SL_DecorationLayer* dec = new SL_DecorationLayer();
    
    if(dec&&dec->init())
    {
        dec->autorelease();
        return dec;
    }
    CC_SAFE_DELETE(dec);
    return nullptr;
}


void SL_DecorationLayer::onEnter()
{
    BaseLayer::onEnter();
    m_pUnitScrollView->setEnable(true);
    
    Button *btn = dynamic_cast<Button *>(this->m_pUILayer->getChildByTag(54));
    btn->setTouchEnabled(true);
    
    if(m_pIndex!=-1 )
    {
     
        m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
        
    }
}
void SL_DecorationLayer::onExit()
{
    BaseLayer::onExit();

}
bool SL_DecorationLayer::init()
{
    bool isInit=false;
    
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        m_pIndex=-1;
        m_bIsTouch = false;
        m_pItemBg= nullptr;
        isInit=true;
   
        
    }while(0);
    
    return isInit;

}
bool SL_DecorationLayer::initUI()
{
    this->addToContentLayer(decorationLayer, 5);

    __String* bgPath;
    bgPath = __String::create("cocostudio/png/Fruit Tart/4/bg.jpg");

    pBgSprite = Utils::getUISprite(bgPath->getCString());
    kAdapterScreen->setExactPosition(pBgSprite, 320,480);
    this->addToBackGroundLayer(pBgSprite, 1);

    //修改路径
    m_pPlate = Sprite::create(g_path + "cream/stands.png");
    m_pPlate->setPosition(Vec2(-50,-200));
    pBgSprite->addChild(m_pPlate,10);
    
    auto cake = Sprite::create(__String::createWithFormat("%scream/cream_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
    cake->setPosition(Vec2(244,350 + 50));
    m_pPlate->addChild(cake);
    cake->setTag(CAKE);
    
    Sprite* pCategoryBgSprite = Utils::getUISprite((g_path_dec + "dec_bg1.png").c_str());
    kAdapterScreen->setExactPosition(pCategoryBgSprite, 320, 0,Vec2(0.5,0),kBorderNone,kBorderBottom);
    this->addToUILayer(pCategoryBgSprite, 9);
//    pCategoryBgSprite->setVisible(false);

    //dec btn Scroll
    m_pGirdView = KDGridView::create(Size(pCategoryBgSprite->getContentSize().width,150));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 30;
    m_pGirdView->m_fMarginLeft = 60;
    m_pGirdView->m_fMarginRight = 60;
    m_pGirdView->setPosition(Vec2(0,100));
    m_pGirdView->setAnchorPoint(Vec2::ZERO);
    pCategoryBgSprite->addChild(m_pGirdView);
    
    m_pAdapter = KSDecorationAdapter::create(m_vTypes);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SL_DecorationLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-1000,0));
    
    //icon btn Scroll
    m_pUnitScrollbgsprite = Utils::getUISprite((g_path_dec + "dec_bg2.png").c_str());
    this->addToUILayer(m_pUnitScrollbgsprite, 5);
    kAdapterScreen->setExactPosition(m_pUnitScrollbgsprite,10,
                                     150,Vec2(1, 1),kBorderRight,kBorderTop);
    
    posIconBg = m_pUnitScrollbgsprite->getPosition();
    m_pUnitScrollbgsprite->setPositionX(m_pUnitScrollbgsprite->getPositionX()+500);
    
    m_pUnitScrollView = UnitScrollView::create(Size(140, 487));
    m_pUnitScrollView->setItemClickCall(CC_CALLBACK_3(SL_DecorationLayer::onUnitCallback, this));
    m_pUnitScrollView->setDirection(extension::ScrollView::Direction::VERTICAL);
    m_pUnitScrollbgsprite->addChild(m_pUnitScrollView);
    m_pUnitScrollView->setAnchorPoint(Vec2::ZERO);
    m_pUnitScrollView->setPosition(Vec2(5,20));
    m_pUnitScrollView->m_pScrollView->setBounceable(true);
    
    
    m_pUnitScrollView->m_iCol = 1;
//    m_pUnitScrollView->m_fHSpace = 20;
    m_pUnitScrollView->m_fVSpace = 20;
//    m_pUnitScrollView->m_fMarginLeft=30;
    m_pUnitScrollView->m_fMarginTop = 10;
    m_pUnitScrollView->m_bNeedAdjust = false;
    
   
    
    resetBtn = ui::Button::create((g_path_ui + "public_btn_reset.png").c_str());
    resetBtn -> setAnchorPoint(Vec2(1, 1));
    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    kAdapterScreen->setExactPosition(resetBtn, 10, 10,Vec2(0, 1),kBorderLeft,kBorderTop);
#else
    kAdapterScreen->setExactPosition(resetBtn, 10, 10,Vec2(0, 1),kBorderLeft,kBorderTop);
#endif
    
    resetBtn->setTag(51);
    this->addToUILayer(resetBtn, 10);
    
    nextBtn = ui::Button::create((g_path_ui + "public_btn_next.png").c_str());
    kAdapterScreen->setExactPosition(nextBtn, 10,10,Vec2(1, 1),kBorderRight,kBorderTop);
    nextBtn->setTag(54);
    this->addToUILayer(nextBtn, 10);
    
    resetBtn -> addTouchEventListener(CC_CALLBACK_2(SL_DecorationLayer::touchEvent, this));
   
    nextBtn  -> addTouchEventListener(CC_CALLBACK_2(SL_DecorationLayer::touchEvent, this));
  
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
    

    if (!m_pTapTitle) {
        m_pTapTitle = Sprite::create(g_path_dec + "text_cake.png");
        kAdapterScreen->setExactPosition(m_pTapTitle,320,0 - 50 - 1000,Vec2(0.5,1),kBorderNone,kBorderTop);
        this->addToUILayer(m_pTapTitle,20);
    }
    
    
   
    
    return true;
}

void SL_DecorationLayer::startAnimtion()
{
    
    ActionHelper::showBezier(m_pPlate, Vec2(333,298 + 50), ActionHelper::ShowDirection::show_from_left);
    
   
    auto funcSounds=CallFunc::create([&](){
        
        
         showBread();
         AudioHelp::getInstance()->playEffect("banner drop.mp3");
        m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 0.5);
        
        m_pTapTitle->runAction(Sequence::create(DelayTime::create(0.5),EaseElasticOut::create(MoveBy::create(1.0,Vec2(0,-1000)), 1.0),CallFunc::create([=](){
        
//            m_pTapTitle->runAction(RepeatForever::create(Sequence::create(MoveBy::create(3.5,Vec2(0,20)),MoveBy::create(3.5,Vec2(0,-20)), NULL)));
//            m_pTapTitle->runAction(RepeatForever::create(Sequence::create(RotateTo::create(3.5,5),RotateTo::create(3.5,-5), NULL)));
            
            
            
        
        }), NULL));
        
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.25),funcSounds, NULL));

//    AudioHelp::getInstance()->playEffect("banner drop.mp3");

}
 void SL_DecorationLayer::showBread()
{
  
   
}

bool  SL_DecorationLayer::initData()
{
    m_vTypes.clear();
    for (int i = 0; i<DEC_ITEM_NUMS; i++) {
        __String* str = __String::createWithFormat("%sdec_icon_%s.png",g_path_dec.c_str(),DEC_ITEM[i].c_str());

        log("%s",str->getCString());
        
        Sprite* data = Sprite::create(str->getCString());
        m_vTypes.pushBack(data);
    }

    return true;
}

void SL_DecorationLayer::onTypeCallback(Ref* sender, Control::EventType controlEvent)
{
    int tag = dynamic_cast<Node*>(sender)->getTag();
    log("button----->%d",tag);

}

void SL_DecorationLayer::onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
//    int tag = dynamic_cast<Node*>(sender)->getTag();
//    if (tag==0) {
//        Vec2 point = m_pGirdView->m_pScrollView->getContentOffset();
//        point.y = point.y+150;
//        if (point.y<=0) {
//            m_pGirdView->m_pScrollView->setContentOffset(point);
//        }else{
//            point.y = 0;
//            m_pGirdView->m_pScrollView->setContentOffset(point);
//        }
//    }else{
//        Vec2 point = m_pGirdView->m_pScrollView->getContentOffset();
//        point.y = point.y-150;
//        if (point.y>(-m_pGirdView->m_pScrollView->getContentSize().height+m_pGirdView->m_pScrollView->getViewSize().height)) {
//            m_pGirdView->m_pScrollView->setContentOffset(point);
//        }else{
//            point.y = -m_pGirdView->m_pScrollView->getContentSize().height+m_pGirdView->m_pScrollView->getViewSize().height;
//            m_pGirdView->m_pScrollView->setContentOffset(point);
//        }
//    }

}

void SL_DecorationLayer::_onTypePageCallback(Ref* sender, Control::EventType controlEvent)
{
    
}
void SL_DecorationLayer::scrollViewTouchBegin(Vec2 worldPoint)
{
    
}
void SL_DecorationLayer::scrollViewWillScroll()
{
    
}
//GirdView点击回调
void SL_DecorationLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
//    
////    m_bIsTouch = false ;
//    if(!m_bIsTouch)
//        return;
    if(m_pTapTitle)
    {
    
        m_pTapTitle->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0, Vec2(0,1000)), 1.0),RemoveSelf::create(),CallFunc::create([=](){
        
            m_pTapTitle = nullptr;
        
        }), NULL));
    }
    
    AudioHelp::getInstance()->playSelectEffect();
    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    if(m_pCurrentSprite&&m_pIndex!=-1)
    {
        m_pCurrentSprite->setColor(Color3B::WHITE);
        
        m_bIsShowUnit = false;
        
        for(auto btn : m_vTypes){
           btn->setColor(Color3B::WHITE);
        }
        
    }
    if(item!=m_pCurrentSprite)
    {

//        item->setColor(Color3B::GRAY);
        float timeScale = 1.2;
        ScaleTo *lScale1 = ScaleTo::create(0.13*timeScale, 1.0, 0.80);
        ScaleTo *lScale2 = ScaleTo::create(0.11*timeScale, 0.82, 1.0);
        ScaleTo *lScale6 = ScaleTo::create(0.07*timeScale, 1.0, 1.0);
        item->runAction(Sequence::create(lScale1, lScale2, lScale6,CallFunc::create([=](){
        
            item->setColor(Color3B::GRAY);
        
        }), NULL));
        m_pItemBg = nullptr;
        m_pUnitScrollView->loadingUnitScrollViewWithType(index);
//        m_pUnitScrollView->m_pScrollView->setContentOffset(Vec2(0,50));
        
        Vec2 offsetPos = m_pUnitScrollView->m_pScrollView->getContentOffset();
        m_pUnitScrollView->m_pScrollView->setContentOffset(Vec2(0,50));
        m_pUnitScrollView->m_pScrollView->setContentOffsetInDuration(offsetPos, 0.25);
//        m_pUnitScrollView->setVisible(true);
//        m_pUnitScrollbgsprite->setVisible(true);
        m_pGirdView->setEnable(false);
//        m_bIsTouch = false;
        showIconBg();
        
        m_pCurrentSprite = item;
        m_pIndex = index;
    }
    else
    {
        m_pCurrentSprite->setNormalImage(Sprite::create(__String::createWithFormat("%sdec_icon_%s.png",
                                                                                   g_path_dec.c_str(),
                                                                                   DEC_ITEM[m_pIndex].c_str())->getCString()));
        m_bIsShowUnit = false;
        m_pCurrentSprite->setColor(Color3B::WHITE);
        for(auto btn : m_vTypes){
            btn->setColor(Color3B::WHITE);
        }
//        m_pUnitScrollView->setVisible(false);
//        m_pUnitScrollbgsprite->setVisible(false);
        m_pGirdView->setEnable(false);
        
        hideIconBg();
        
        m_pCurrentSprite=nullptr;
        m_pIndex=-1;
        
        
    }
}
void SL_DecorationLayer::showIconBg()
{

    if (m_pUnitScrollbgsprite->getPosition() == posIconBg) {
        
        ActionUtils::delayTimeAction(this,0.3,CallFunc::create([=](){
            
            m_pGirdView->setEnable(true);
            
        }));
        return;
        
    }
    
    m_pUnitScrollbgsprite->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(0.5, posIconBg), 1.0),CallFunc::create([=](){
    
        m_pGirdView->setEnable(true);
    
    
    }), NULL));
    
    
}
void SL_DecorationLayer::hideIconBg()
{
    m_pGirdView->setEnable(false);
    if (m_pUnitScrollbgsprite->getPosition() == posIconBg + Vec2(500,0)) {
        
        
        ActionUtils::delayTimeAction(this,0.3,CallFunc::create([=](){
            
            m_pGirdView->setEnable(true);
            
        }));
        
        
        return;
        
    }
    m_pUnitScrollbgsprite->runAction(Sequence::create(EaseElasticIn::create(MoveTo::create(0.5, posIconBg + Vec2(500,0)), 1.0),CallFunc::create([=](){
        
        m_pGirdView->setEnable(true);
        
        
    }), NULL));

}
void SL_DecorationLayer::showHand()
{


}
void SL_DecorationLayer::onUnitCallback(int tag,MenuItem* menuItem,int index)
{
    AudioHelp::getInstance()->playEffect("add.mp3");
    log("----->m_pIndex  %d",m_pIndex);
    if(m_pIndex==-1){
        return;
        
    }
    
    if(m_pCurrentSprite){

        m_pCurrentSprite->setColor(Color3B::WHITE);
        
        m_pCurrentSprite=nullptr;
    }

    MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
    
//    if(kIAPManager->isFree(DEC_ITEM[m_pIndex].c_str(),index))
    if(kIAPManager->isPackagePurchased(2)||index <= DEC_ITEM_Num[m_pIndex]/3)
    {
        if (m_pItemBg) {
            m_pItemBg->removeFromParent();
            m_pItemBg = nullptr;
        }
        
        
        if (!m_pItemBg) {
            m_pItemBg = Sprite::create("cocostudio/png/cake/dec/dec_bg2_xuanzhong.png");
            m_pItemBg->setPosition(item->getContentSize()/2);
            item->addChild(m_pItemBg);
        }
        

        switch (m_pIndex) {
            case  PLATES:
            {
//                m_pUnitScrollView->setEnable(false);
                m_pGirdView->setEnable(false);
                
                auto cone = Sprite::create(__String::createWithFormat("%s%s/%s%d.png",g_path_dec.c_str(),DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                cone->setPosition(pBgSprite->convertToNodeSpace(item->getParent()->convertToWorldSpace(item->getPosition())));
//                this->addToContentLayer(cone, 5);
                pBgSprite->addChild(cone, 0);
                cone->setScale(0.5);
                
                g_dec_plates = index;
                
                auto func = CallFunc::create([=](){
                
                    log("0000");
                    auto path = __String::createWithFormat("%s%s/%s%d.png",g_path_dec.c_str(),DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString();
                    m_pPlate->setTexture(path);
                    WeddingFoodData::setPlatePath(path);
                    m_pUnitScrollView->setEnable(true);
                    m_pGirdView->setEnable(true);
                    
                    log("11111");
                    
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");

                    emitter1->setPosition(decorationLayer->convertToWorldSpace(m_pPlate->getPosition())+Vec2(-50,0));
                    
                    emitter1->setAutoRemoveOnFinish(true);
                    this->addChild(emitter1, 300);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
//                    m_pIndex=-1;
                    
                });
                ScaleTo* scale = ScaleTo::create(0.5,1.0);

                
                
                
//                ActionHelper::showBezier(cone, pBgSprite->convertToWorldSpace(m_pPlate->getPosition()), ActionHelper::ShowDirection::show_from_right,[this](){
//                
////                    cone->runAction(Sequence::create(scale,CallFunc::create([=](){
////                    
////                    
////                        
////                    
////                    }),NULL));
////                
//                
//                });
                
                MoveTo* move = MoveTo::create(0.5, m_pPlate->getPosition());
                Spawn* sp = Spawn::create(scale,move, NULL);
                cone->runAction(Sequence::create(BezierCreator::createParabola(1.0, cone->getPosition(), decorationLayer->convertToWorldSpace(m_pPlate->getPosition())),scale,RemoveSelf::create(),func,NULL));
                
                
            }
                break;
            default:
            {
                m_pGirdView->setEnable(false);
                m_pUnitScrollView->setEnable(false);
                
                int v_rand = rand()%4 * 45;
                
                auto ice = FillMaterialModel::create(__String::createWithFormat("%s%s/%s%d.png",g_path_dec.c_str(),DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
               
                log("%s",__String::createWithFormat("%s%s/%s%d.png",g_path_dec.c_str(),DEC_ITEM[m_pIndex].c_str(),DEC_ITEM[m_pIndex].c_str(),index)->getCString());
                
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
                    ParticleSystemQuad *emitter1 = ParticleSystemQuad::create("cocostudio/particles/decorateParticle.plist");
                    kAdapterScreen->setExactPosition(emitter1, 580 + v_rand, 450);

                    emitter1->setPosition(v);
                    emitter1->setAutoRemoveOnFinish(true);
                    this->addChild(emitter1, 30);
                    AudioHelp::getInstance()->playEffect("all star .mp3");
//                    m_pIndex=-1;
                    
                });
                
                MoveTo* move = MoveTo::create(0.5, v);
                ScaleTo* scale = ScaleTo::create(0.5, 1);
                Spawn* spwn = Spawn::create(BezierCreator::createParabola(1.0, ice->getPosition(), v),scale, NULL);
                ice->runAction(Sequence::create(MoveTo::create(0, item->getParent()->convertToWorldSpace(item->getPosition())),spwn,func,NULL));
            }
                break;
       
        }
    }else{
    
        m_pUnitScrollView->setEnable(false);
//        SceneManager::getInstance()->enterShopScene();
        
        ShopLayer* shop = ShopLayer::create();
        this->addChild(shop,300);
        shop->_callBack=[=](){
        
             m_pUnitScrollView->setEnable(true);
            if(m_pIndex!=-1 )
            {
                
                m_pUnitScrollView->loadingUnitScrollViewWithType(m_pIndex);
                m_pItemBg = nullptr;
            }
        
        };
    }

}

void SL_DecorationLayer::onBtnCallBack(Ref* sender)
{
    
}

void SL_DecorationLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
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
                    Dialog* dialog= Dialog::create(Size(517,452), (void*)kDialogReset, Dialog::DIALOG_TYPE_NEGATIVE);
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
                        if (data == 1) {
                            
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->stopTipsFrameStatus();
                            dynamic_cast<FillMaterialModel*>(notEatLayer->getChildren().at(i))->setNotDeleteSelf();
                        }
                    }
                    
                    //next
                    
                    auto cake = Sprite::create(__String::createWithFormat("%scream/cream_%s.png",g_path.c_str(),g_choose_waste.c_str())->getCString());
                    cake->setPosition(this->convertToWorldSpace(m_pPlate->convertToWorldSpace(Vec2(244,350 + 50))));
                    
                    RenderTexture *rt = RenderTexture::create(visibleSize.width, visibleSize.height);
                    rt->begin();
                    
                    //    pCone->visit();
                    cake->visit();
                    
                    decorationLayer->visit();
                    
                    
                    rt->end();
                    Director::getInstance()->getRenderer()->render();
                    WeddingFoodData::saveFood(rt);
//                        Director::getInstance()->pushScene(EatLayer::createScene());
                  //SceneChangeManager->enterEatScene();
                    
                    
                    auto  _endFunc = [this](){
                        LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                        Director::getInstance()->pushScene(TransitionFade::create(0.8,EatLayer::createScene(),Color3B::WHITE));
                    };
                    
                    if(IAPManager::getInstance()->isShowAds()){
                        LQ_adLoadingLayer::loadingDoneCallback = _endFunc;
                        LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
                    }
                    else
                        _endFunc();
                    
                }
                    
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    
}
void SL_DecorationLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{

    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(517,452), (void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addToUILayer(dialog, 9999);
        }
    }

}
void SL_DecorationLayer::onNegativeClick(void*)
{
    m_bIsShowReturn = false;
}

void SL_DecorationLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    int lType = (uintptr_t)type;
    if (lType == kDialogReturnHome)
    {
        
        if(kIAPManager->isShowAds())
        {
            
            
            //        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
            
            
        
            LQ_adLoadingLayer::loadingDoneCallback = []()
            {
                LQ_adLoadingLayer::loadingDoneCallback = nullptr;
                Director::getInstance()->popToRootScene();
                Director::getInstance()->replaceScene(HomeLayer::createScene());
            };
                LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
            
        }
        else
        {
            Director::getInstance()->popToRootScene();
            Director::getInstance()->replaceScene(HomeLayer::createScene());
            
        }

    }
    if (lType == kDialogReset)
    {
        AudioHelp::getInstance()->playEffect("reset.mp3");
        WeddingFoodData::setPlatePath("");
        
       
        g_dec_bg = -1;
        g_dec_plates = -1;
        g_eat_cake = false;
        m_pPlate->getChildByTag(CAKE)->setVisible(true);
        m_pPlate->setTexture(g_path + "cream/stands.png");
        decorationLayer->removeAllChildren();
        hideIconBg();
        
    }
    
}




