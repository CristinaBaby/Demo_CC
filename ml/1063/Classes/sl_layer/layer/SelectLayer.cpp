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
#include "ChangeLoading.h"
#include "MainScene.h"
#include "LQ_adLoadingLayer.h"
#include "HomeLayer.h"
REGIST_SECENE(SelectLayer);
string tastPath[5]={"chocolate","vanilla","strawberry","greentea","blueberry"};

SelectLayer::SelectLayer()
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
        m_pTapTitle = nullptr;
        isInit=true;
        
    }while(0);
    
    return true;
    
}

void SelectLayer::initData()
{

    m_vTypes.clear();
    for (int i = 0; i<5; i++) {
        
        __String* str = __String::createWithFormat("%sselect/choose_%s.png",g_path.c_str(),tastPath[i].c_str());
        Sprite* data = Sprite::create(str->getCString());
        data->setTag(i);
        
        m_vTypes.pushBack(data);
    }
}
void SelectLayer::initUI()
{
    __String* bgPath;
    
    auto size = Director::getInstance()->getOpenGLView()->getFrameSize();
    auto max = std::max(size.height, size.width);
    float scale = 1.0;
    if(max>=2048){

        bgPath = __String::create("cocostudio/png/cookies/1/bg_h.jpg");
        scale = 0.5;
    }else{
    
        bgPath = __String::create("cocostudio/png/cookies/1/bg.jpg");
    
    }
    
    auto _bg = Sprite::create(bgPath->getCString());
    kAdapterScreen->setExactPosition(_bg, 320, 480);
    this->addChild(_bg);
    _bg->setScale(scale);
    
    Sprite* bg = Sprite::create("cocostudio/png/cookies/1/bg.jpg");
    kAdapterScreen->setExactPosition(bg, 320, 480);
    this->addChild(bg);
//    bg->setScale(scale);
    bg->setVisible(false);

    m_pGirdView = KDGridView::create(Size(Director::getInstance()->getVisibleSize().width, 600));
    m_pGirdView->setDirection(extension::ScrollView::Direction::HORIZONTAL);
    m_pGirdView->m_iRow = 1;
    m_pGirdView->m_fHSpace = 100;
//    m_pGirdView->m_fVSpace = 20;
    m_pGirdView->m_fMarginTop = 50;
    m_pGirdView->m_fMarginRight = 100;
    m_pGirdView->m_fMarginLeft = 100;
    
    m_pGirdView->setEnable(false);
    kAdapterScreen->setExactPosition(m_pGirdView, 0, 250,Vec2(0,0),kBorderLeft,kBorderBottom);
    this->addChild(m_pGirdView,7);
    
    m_pGirdView->setPositionY(bg->convertToWorldSpace(Vec2(0,325)).y);
    m_pAdapter = KSSimpleAdapter::create(m_vTypes);
    m_pAdapter->setIapType(0);
    m_pGirdView->setAdapter(m_pAdapter);
    m_pGirdView->setItemClickCall(CC_CALLBACK_3(SelectLayer::typeItemClick, this));
    m_pGirdView->m_pScrollView->setBounceable(true);
    m_pGirdView->m_bNeedAdjust = false;
    
    nextBtn = ui::Button::create(g_path_ui + "public_btn_next.png");
    nextBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::touchEvent, this));
    nextBtn->setTag(100);
    nextBtn->setVisible(false);
    kAdapterScreen->setExactPosition(nextBtn, 10 + nextBtn->getContentSize().width/2, 110 + nextBtn->getContentSize().height/2,Vec2(0.5, 0.5),kBorderRight,kBorderBottom);
    this->addToUILayer(nextBtn, 20);

    auto light = Sprite::create("cocostudio/ui/public/light.png");
    light->setPosition(nextBtn->getContentSize()/2);
    nextBtn->addChild(light,-1);
    
    light->runAction(RepeatForever::create(RotateBy::create(1,90)));

    nextBtn->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2),Sequence::create(ScaleTo::create(0.3, 1.1),ScaleTo::create(0.3, 1), nullptr), nullptr)));
    
    chooseParticle = ParticleSystemQuad::create("cocostudio/particles/knifeParticl.plist");
    chooseParticle->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
    chooseParticle->retain();
    
    m_pGirdView->m_pScrollView->setContentOffset(Vec2(-520,0));
    
    auto container = m_pGirdView->m_pScrollView->getContainer();
    int i = 0;
    for(auto select : m_vTypes){
    
        log("%d",select->getTag());
        Sprite* brand = Sprite::create(__String::createWithFormat("%sselect/brand_%s.png",g_path.c_str(),tastPath[i].c_str())->getCString());
        brand->setPosition(container->convertToNodeSpace(select->convertToWorldSpace(Vec2(43,35 + 10))));
        container->addChild(brand,50);
        i++;
    }
    
}
void SelectLayer::onEnter()
{
    BaseLayer::onEnter();
    
    
    m_pGirdView->refresh();
    m_pCurrentSprite=nullptr;
}
void SelectLayer::onEnterTransitionDidFinish()
{
    BaseLayer::onEnterTransitionDidFinish();
    chooseAction();
    showTap();
    if (kIAPManager->isShowAds()) {
        
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
    
}
void SelectLayer::onExit()
{
    BaseLayer::onExit();
}
void SelectLayer::titleAction()
{

}
void SelectLayer::showTap()
{
    if(!m_pTapTitle){
    
        m_pTapTitle = Sprite::create("cocostudio/ui/tips/text_flavor.png");
        kAdapterScreen->setExactPosition(m_pTapTitle, 320, -100 - 1000 ,Vec2(0.5, 1),kBorderNone,kBorderTop);
        this->addChild(m_pTapTitle,200);
        
        ActionUtils::moveByElasticOut(m_pTapTitle, Vec2(0,-1000));
        
    }
}
void SelectLayer::fontAction()
{
    m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(0,0), 0.5);
    auto func = CallFunc::create([=](){
        m_pGirdView->setEnable(true);
    });
    this->runAction(Sequence::create(DelayTime::create(0.5),func, NULL));
    
}
void SelectLayer::chooseAction()
{
   m_pGirdView->m_pScrollView->setContentOffsetInDuration(Vec2(-1020,0), 1.0);
    
    
   auto func = CallFunc::create([=](){
   
       fontAction();
       
   });
    this->runAction(Sequence::create(DelayTime::create(1.0),func, NULL));
}
void SelectLayer::typeItemClick(int tag,MenuItem* menuItem,int index)
{
    
    if(kIAPManager->isFree("cake_select",index)){
    
        if(m_pTapTitle){
        
            m_pTapTitle->runAction(Sequence::create(EaseElasticIn::create(MoveBy::create(1.0,Vec2(0,1000)),1.0),RemoveSelf::create(),CallFunc::create([=](){
            
                m_pTapTitle = nullptr;
            
            }),nullptr));
        
        }
        MenuItemSprite* item = dynamic_cast<MenuItemSprite*>(menuItem);
        int y = item->getNormalImage()->getPositionY();
        
        
        log("------>%f",m_pGirdView->m_pScrollView->getContentOffset().x);
        
        //判断是否任务
        if (m_iTouchIndex == index) {
            return;
        }else if(m_pCurrentSprite){
            
            m_pCurrentSprite->getNormalImage()->stopAllActions();
            m_pCurrentSprite->getNormalImage()->setPositionY(y);
            
        }
        
        m_pCurrentSprite = item;
        m_pIndex = index;
        
        m_iTouchIndex = index;
        
        
        g_choose_waste = tastPath[index];
        log("%s",g_choose_waste.c_str());
        
        chooseParticle->removeFromParent();
        chooseParticle->setVisible(true);
        chooseParticle->setPosition(item->getContentSize()*.5);
        chooseParticle->setScale(1.5);
        item->addChild(chooseParticle,4);
        
        AudioHelp::getInstance()->playEffect("choose.mp3");
        
        nextBtn->setVisible(true);
        
        
        
        ActionUtils::jumpAction(item->getNormalImage(), 2,nullptr,0.4);
    
    }else{
        m_pGirdView->setEnable(false);
        SceneChangeManager->enterShopScene();
    
    }
    
    
   
}
void SelectLayer::onKeyReleasedCallBack(EventKeyboard::KeyCode keyCode,Event* event)
{
    if(keyCode==EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!m_bIsShowReturn)
        {
            m_bIsShowReturn = true;
            Dialog* dialog= Dialog::create(Size(517,452), (void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE);
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

void SelectLayer::onNegativeClick(void* type)
{
    m_bIsShowReturn = false;
}
void SelectLayer::onPositiveClick(void* type)
{
    m_bIsShowReturn = false;
    
    if(kIAPManager->isShowAds())
    {
        
       
//        AdLoadingLayerBase::showLoading<AdsLoadingLayer>(true);
        
        
        LQ_adLoadingLayer::showLoading<LQ_adLoadingLayer>(true);
        LQ_adLoadingLayer::loadingDoneCallback = []()
        {
            LQ_adLoadingLayer::loadingDoneCallback = nullptr;
            Director::getInstance()->popToRootScene();
            Director::getInstance()->replaceScene(HomeLayer::createScene());
        };
        
    }
    else
    {
        Director::getInstance()->popToRootScene();
        Director::getInstance()->replaceScene(HomeLayer::createScene());
        
    }
    
}
void  SelectLayer::touchEvent(Ref *obj , Widget::TouchEventType type)
{
    Button *btn = dynamic_cast<Button *>(obj);
    switch (type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            switch (btn->getTag()) {
                case 100:
                {
                      AudioHelp::getInstance()->playEffect("button_general.mp3");
//                    SceneChangeManager->enterGameScene();
                      SceneChangeManager->enterMakeScene();
//                    ChangeLoading::loading(nullptr);
                    

                    
                }
                    break;
            }
        }
    }

}
