        //
//  SelectLayer.cpp
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#include "SelectLayer.h"
#include "Global.h"
#include "IAPManager.h"
#include "ActionUtils.h"
#include "AudioHelp.h"
#define CardTag 1000

SelectLayer::SelectLayer():m_pTitle(nullptr),nextBtnY(0.0)
{
    
}
SelectLayer::~SelectLayer()
{
    
}
bool SelectLayer::init()
{
    bool isInit=false;
    do{
        
        CC_BREAK_IF(!BaseLayer::init());
        initData();
        initUI();
        
        m_pCurrentSprite = nullptr;
      
        m_pIndex = -1;
        m_iFontNum = -1;
        m_iTouchIndex = -1;
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void SelectLayer::initData()
{
    
}
void SelectLayer::showTap()
{
    if (!m_pTitle) {
        
        m_pTitle = Sprite::create("cocos_studio/studio_ui/choose_flavor/font_choose_a_flavor.png");
        kAdapterScreen->setExactPosition(m_pTitle, 320, 50 - 500,Vec2(0.5,1),kBorderNone,kBorderTop);
        this->addToContentLayer(m_pTitle, 20);
         ActionUtils::dropOut(m_pTitle, Vec2(0,-500));
    }
   
    
}
void SelectLayer::hideTap()
{
    if (m_pTitle) {

        auto func = CallFunc::create([=](){

            m_pTitle->removeFromParent();
            m_pTitle=nullptr;
        
        });
        
        ActionUtils::dropOut(m_pTitle, Vec2(0,500),1.0,func);
        
    }

}
void SelectLayer::initUI()
{
    auto ui_button = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("cocos_studio/choose_flavors.json");
    this->addChild(ui_button,10);

    ui_number= (TextAtlas*)ui::Helper::seekNodeByName(ui_button, "number");
    ui_number->setString(__String::createWithFormat("%d",UserDefault::getInstance()->getIntegerForKey(CoinNum))->getCString());
    ui_number->setPositionX(ui_number->getPositionX()-10);
    
    Sprite* bg = Sprite::create("images/select/bg_choose.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width, 621));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 10;
//    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 200;
//    m_pGirdView->m_fMarginBottom = 100;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;

    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 250,Vec2(0,0),kBorderLeft,kBorderBottom);
//    m_pGirdView->setPosition(kAdapterScreen->getExactPostion(Vec2(0,465 + 100)));
//    m_pGirdView->setAnchorPoint(Vec2(0,0.5));
    this->addChild(m_pGirdView,7);
    
    Vec2 pos = bg->convertToWorldSpace(Vec2(0,328));
    m_pGirdView->setPositionY(pos.y);
//    this->addToContentLayer(m_pGirdView);
//    float
    
    
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(0);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SelectLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    nextBtn = (Widget*)ui::Helper::seekNodeByName(ui_button, "btn_next");
    nextBtn->setTag(100);
    nextBtn->setVisible(false);
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    nextBtnY = nextBtn->getPositionY();
    chooseParticle = ParticleSystemQuad::create("particle/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-520,0));
    
    
    for (int i = 0; i<m_vTypes.size(); i++) {
        auto card = Sprite::create(__String::createWithFormat("images/select/flavor_card/card%d.png",i)->getCString());
        card->setAnchorPoint(Vec2(0.48,0.03));
        card->setTag(CardTag);
        card->setPosition(Vec2(127,337));
        m_vTypes.at(i)->addChild(card);
     }
    
    
    
    
    
}
void SelectLayer::onEnter()
{
    BaseLayer::onEnter();
    m_pGirdView->refresh();
    m_pCurrentSprite=nullptr;
    showTap();
  
}
void SelectLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    chooseAction();
    if (kIAPManager->isShowAds()) {
        
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
     ui_number->setString(__String::createWithFormat("%d",UserDefault::getInstance()->getIntegerForKey(CoinNum))->getCString());
}
void SelectLayer::onExit()
{
    BaseLayer::onExit();
}
void SelectLayer::titleAction()
{

}
void SelectLayer::fontAction()
{
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 0.5);
    auto func = CallFunc::create([=](){
        m_pGirdView->setEnable(true);
    });
    this->runAction(Sequence::create(DelayTime::create(1.0),func, NULL));
    
}
void SelectLayer::chooseAction()
{
   m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-1020,0), 1.0);
    
    
   auto func = CallFunc::create([=](){
//       m_pGirdView->setEnable(true);
       fontAction();
       
   });
    this->runAction(Sequence::create(DelayTime::create(1.5),func, NULL));
}
void SelectLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
}
//设置口味的数据，存入一个全局容器
void SelectLayer::setChooseData(int index)
{
   
}
void SelectLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(450,358), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
            //                    dialog -> setAnchorPoint(Vec2(0,0));
            //                    STVisibleRect::setPosition(dialog, size.width/2, size.height/2);
            dialog -> setAnchorPoint(Vec2(0,0));
            kAdapterScreen->setExactPosition(dialog, 0, 0,Vec2(0, 0),kBorderLeft,kBorderBottom);
            dialog->setContentText("Are you sure you want to start over?");
            dialog->setPositiveBtnText("");
            dialog->setNegativeBtnText("");
            dialog->setCallback(this);
            this->addChild(dialog, 9999);
        }
    }
}

void SelectLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void SelectLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    
    if(kIAPManager->isShowAds())
    {
        AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
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
void SelectLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                    
                }
                    break;
            }
        }
    }
}
