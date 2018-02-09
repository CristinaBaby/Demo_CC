//
//  MakeMoldScene.cpp
//  MLABS1061_GummyMaker
//
//  Created by wusonglin1 on 15/12/21.
//
//

#include "MakeMoldScene.h"
#include "IAPManager.h"
#include "AdsManager.h"

MakeMoldScene::MakeMoldScene()
{
    
}
MakeMoldScene::~MakeMoldScene()
{
    
}
bool MakeMoldScene::init()
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(!GameBaseScene::init());
        initUI();
        bRet = true;
    } while (0);
    return bRet;
}
void MakeMoldScene::onEnter()
{
    GameBaseScene::onEnter();
    
    FlurryEventManager::getInstance()->logCurrentModuleEnterEvent("Mold stage");
    if (kIAPManager->isShowAds()) {
        //        STAds st;
        //        st.requestAds();
        AdsManager::getInstance()->preloadAds(ADS_TYPE::kTypeBannerAds);
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    }
}
void MakeMoldScene::onEnterTransitionDidFinish()
{
    GameBaseScene::onEnterTransitionDidFinish();
}
void MakeMoldScene::onExit()
{
    GameBaseScene::onExit();
    FlurryEventManager::getInstance()->logCurrentModuleExitEvent("Mold stage");
}
void MakeMoldScene::initUI()
{
    m_pBG = Sprite::create("images/make_mold/select/choose_bg.jpg");
    kAdapterScreen->setExactPosition(m_pBG,480,320);
    this->addToBGLayer(m_pBG,0);
    
    //选择mold的Layer
    m_pSelectMoldLayer= SelectMoldLayer::create();
    this->addChild(m_pSelectMoldLayer,10);
    m_pSelectMoldLayer->changeBgCallback=[=](){

//        m_pBG->setTexture("images/cook_mold/make_bg.jpg");
        
        auto bg = Sprite::create("images/cook_mold/make_bg.jpg");
        m_pBG->addChild(bg);
        bg->setAnchorPoint(Vec2::ZERO);
        bg->setOpacity(0);
        bg->runAction(FadeIn::create(0.3));
        
        
    };
    m_pSelectMoldLayer->showPourLayer=[=](){
    
        //机器的Layer
        m_pPourMoldLayer = PourMoldLayer::create();
        this->addChild(m_pPourMoldLayer,5);
    
        m_pPourMoldLayer->selectMoldHideCallback=[=](){
            
            m_pSelectMoldLayer->hideSelectBtn();
            
        };
        
        m_pSelectMoldLayer->startPourCallback=[=](int tag){
            
            //此刻把逻辑交给pour
            m_pPourMoldLayer->startPour(tag,m_pBG->convertToWorldSpace(Vec2(550,290)));
            m_pSelectMoldLayer->_hideSelectBtn();
        };
    };
    m_pSelectMoldLayer->isSelectCallback=[=](){
    
        m_pPourMoldLayer->showPourMac();
    
    };
   
    
}
void MakeMoldScene::nextStep()
{
    log("next");
    hideNextButton();
    
}