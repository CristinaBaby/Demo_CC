//
//  HomeLayer.cpp
//  FastFood
//
//  Created by huxixiang on 15/8/3.
//
//

#include "HomeLayer.h"
#include "CFSystemFunction.h"
#include "IAPManager.h"
#include "Global.h"
#include "AdsManager.h"
#include "AdsLoadingLayer.h"
#include "MoreGameLoader.h"
#include "SettingLayer.hpp"

enum HomeLayer_Tag{
    eStartBtn_tag,
    eMoreBtn_tag,
    eShopBtn_tag
};

std::string logoPath[]{
    "images/home/logo_i.png",
    "images/home/logo_c.png",
    "images/home/logo_e.png",
    "images/home/logo_c2.png",
    "images/home/logo_r.png",
    "images/home/logo_e2.png",
    "images/home/logo_a.png",
    "images/home/logo_m.png",
    "images/home/logo_2.png",
    "images/home/logo_ice cream.png"
};

Vec2 logoCoordinate[]{
    Vec2(158.5f, 518.0f),
    Vec2(209.0f, 519.0f),
    Vec2(240.5f, 535.0f),
    Vec2(313.0f, 526.5f),
    Vec2(360.0f, 504.0f),
    Vec2(410.5f, 501.0f),
    Vec2(454.0f, 498.5f),
    Vec2(521.5f, 521.0f),
    Vec2(586.5f, 516.5f),
    Vec2(403.0f, 573.0f)
};

Scene* HomeLayer::scene()
{
    auto pScene = Scene::create();
    auto pLayer = HomeLayer::create();
    pScene->addChild(pLayer);
    
    return pScene;
}


HomeLayer::HomeLayer():m_pBg(nullptr),isReturn(false),m_bIsStartKey(true),m_iCount(0),m_bIsFlag(false),m_pWater1(nullptr),m_pWater2(nullptr),m_pTarget(nullptr)
{

}

HomeLayer::~HomeLayer()
{

}

bool HomeLayer::initData()
{

    return true;
}

bool HomeLayer::init()
{
    bool isInit = false;
    do
    {
        CC_BREAK_IF(!Layer::init());
        CC_BREAK_IF(!initLayer());

        SoundPlayer::getInstance()->playBackGroundMusic(startBgMusick);
        g_bBgMusicType = false;
        this->setKeypadEnabled(true);
        isInit = true;
    }while(0);
    
    return isInit;
}

bool HomeLayer::initLayer()
{
    auto pLockLayer = LockScreenLayer::create();
    this->addChild(pLockLayer, INT16_MAX);
    
    // add bg
    m_pBg = Sprite::create("images/home/bg.jpg");
    kAdapterScreen->setExactPosition(m_pBg, 480, 320);
    this->addChild(m_pBg);
    
  
    /////////////////////////////////////////////////////////////////////
    // add start button
    auto pStartBtn = ToolSprite::create("images/home/play.png");
    kAdapterScreen->setExactPosition(pStartBtn, 480, 210);
    pStartBtn->setDelegate(this);
    pStartBtn->setTag(eStartBtn_tag);
    pStartBtn->setShadeBtn(true);
    this->addChild(pStartBtn, 100);
    pStartBtn->setScale(0);
    pStartBtn->setRotation(-250);

    ////////////////////////////////////////////////////////////////
    // add clipping node
    auto stencil = Sprite::create("images/home/play_mask.png");
    stencil->setPosition(80, 80);
    
    auto pClipNode = ClippingNode::create();
    pClipNode->setContentSize(Size(160, 160) / 2);
    pClipNode->setStencil(stencil);
    pClipNode->setInverted(false);
    pClipNode->setAlphaThreshold(0);
    pClipNode->ignoreAnchorPointForPosition(false);
    pClipNode->setAnchorPoint(Point(0.5f, 0.5f));
    pClipNode->setPosition(60, 80);
    pStartBtn->addChild(pClipNode);
    
    m_pTarget = Sprite::create("images/home/play_1.png");
    m_pTarget->setPosition(Size(80 + 200, 10));
    pClipNode->addChild(m_pTarget);
    
    
    pStartBtn->runAction(Sequence::create(DelayTime::create(1.0f), Spawn::create(ScaleTo::create(0.7, 1.08f), RotateBy::create(0.7f, 250), nullptr), CallFunc::create([=](){
        
        pStartBtn->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.0f, 1.0f), ScaleTo::create(1.0f, 1.1f), nullptr)));
        pLockLayer->removeFromParentAndCleanup(true);
        
        m_pTarget->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(-100, 0)), CallFunc::create([=](){
        
            m_pTarget->runAction(RepeatForever::create(Sequence::create(MoveBy::create(4.0f, Vec2(-200, 0)), MoveBy::create(4.0f, Vec2(200, 0)), nullptr)));
        }), nullptr));
        
    }), nullptr));
    

    // add moregame button
    auto pMoreBtn = ToolSprite::create("images/home/btn_more.png");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    kAdapterScreen->setExactPosition(pMoreBtn, 20 + pMoreBtn->getContentSize().width / 2, 18 + pMoreBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderBottom);
#endif
    pMoreBtn->setDelegate(this);
    pMoreBtn->setTag(eMoreBtn_tag);
    pMoreBtn->setShadeBtn(true);
    this->addChild(pMoreBtn, 100);
 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    kAdapterScreen->setExactPosition(pMoreBtn, 20 + pMoreBtn->getContentSize().width / 2 + 110, 18 + pMoreBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderBottom);
    
    // add shop button
    auto pShopBtn = ToolSprite::create("images/home/btn_shop.png");
    kAdapterScreen->setExactPosition(pShopBtn, 20 + pShopBtn->getContentSize().width / 2, 15 + pShopBtn->getContentSize().height / 2, Vec2(0.5f, 0.5f), kBorderRight, kBorderBottom);
    pShopBtn->setDelegate(this);
    pShopBtn->setTag(eShopBtn_tag);
    pShopBtn->setShadeBtn(true);
    this->addChild(pShopBtn, 100);
#endif

    this->addAnimation();
    
    return true;
}

void HomeLayer::addAnimation()
{
    // logo
    for (int i = 0; i < 10; i++) {
        auto pLogoLetter = ToolSprite::create(logoPath[i].c_str());
        pLogoLetter->setPosition(logoCoordinate[i] + kAdapterScreen->g_oOffset);
        if (i < 3) {
            this->addChild(pLogoLetter, 100 - i);
        }else{
            this->addChild(pLogoLetter, 100 + i);
        }
        pLogoLetter->setTag(100 + i);
    }
 
    // 人物
    auto pGirl = Sprite::create("images/home/girl_2.png");
    kAdapterScreen->setExactPosition(pGirl, 800, 275);
    this->addChild(pGirl, 2);
    
    auto pAnimation = Animation::create();
    pAnimation->addSpriteFrameWithFile("images/home/girl_2.png");
    pAnimation->addSpriteFrameWithFile("images/home/girl_1.png");
    pAnimation->setDelayPerUnit(0.8f);
    pAnimation->setRestoreOriginalFrame(false);
    auto pAnimate = Animate::create(pAnimation);
    pGirl->runAction(RepeatForever::create(pAnimate));
}

void HomeLayer::logoAnimation(float ft)
{
    if (!m_bIsFlag) {
        m_bIsFlag = true;
        if (m_pWater1) {
            m_pWater1->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([=](){
                m_pWater1->removeFromParentAndCleanup(true);
                m_pWater1 = nullptr;
            }), nullptr));
        }
        if (m_pWater2) {
            m_pWater2->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.3f), CallFunc::create([=](){
                m_pWater2->removeFromParentAndCleanup(true);
                m_pWater2 = nullptr;
            }), nullptr));
        }
        
        auto pAction1 = Sequence::create(Spawn::create(RotateBy::create(0.15f, 15), MoveBy::create(0.15f, Vec2(0, 25)), nullptr), Spawn::create(RotateBy::create(0.1f, -15), MoveBy::create(0.1f, Vec2(0, -25)), nullptr), nullptr);
        auto pAction2 = Sequence::create(MoveBy::create(0.15f, Vec2(0, 25)), MoveBy::create(0.1f, Vec2(0, -25)), nullptr);
        auto pAction3 = Sequence::create(Spawn::create(RotateBy::create(0.15f, -15), MoveBy::create(0.15f, Vec2(0, 25)), nullptr), Spawn::create(RotateBy::create(0.1f, 15), MoveBy::create(0.1f, Vec2(0, -25)), nullptr), nullptr);
        this->getChildByTag(109)->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f), pAction1, pAction2, pAction3, DelayTime::create(0.5f), nullptr)));
    }
    
    this->getChildByTag(100 + m_iCount)->setLocalZOrder(this->getChildByTag(100 + m_iCount)->getLocalZOrder() + 20);
    this->getChildByTag(100 + m_iCount)->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.2f, 1.1f), MoveBy::create(0.2f, Vec2(0, 25)), nullptr), CallFunc::create([=](){
    
        this->getChildByTag(100 + m_iCount)->setLocalZOrder(this->getChildByTag(100 + m_iCount)->getLocalZOrder() - 20);
        
        m_iCount++;
        if (m_iCount >= 9) {
            this->unschedule(schedule_selector(HomeLayer::logoAnimation));
            this->getChildByTag(109)->stopAllActions();
            this->getChildByTag(109)->setPosition(static_cast<ToolSprite*>(this->getChildByTag(109))->getOriginPoint());
            this->getChildByTag(109)->setRotation(0);
            m_iCount = 0;
            
            m_bIsFlag = false;
            this->scheduleOnce([=](float ft){
            
                this->schedule(schedule_selector(HomeLayer::logoAnimation), 0.25f);
            }, 8.0f, "ss");
            
            if (!m_pWater1) {
                m_pWater1 = Sprite::create("images/home/water_0.png");
                m_pWater1->setPosition(40, 30);
                this->getChildByTag(101)->addChild(m_pWater1);
                m_pWater1->setOpacity(0);
                m_pWater1->runAction(Sequence::create(FadeIn::create(0.5f), CallFunc::create([=](){
                
                    auto pFlash = Sprite::create("images/home/flash.png");
                    pFlash->setPosition(8, 5);
                    m_pWater1->addChild(pFlash);
                    pFlash->setOpacity(0);
                    pFlash->runAction(RepeatForever::create(Sequence::create(Spawn::create(RotateBy::create(1.0f, 180), FadeIn::create(1.0f), nullptr), FadeOut::create(0.6f), nullptr)));
                }), nullptr));
            }
            
            if (!m_pWater2) {
                m_pWater2 = Sprite::create("images/home/water_1.png");
                m_pWater2->setPosition(30, 75);
                this->getChildByTag(108)->addChild(m_pWater2);
                m_pWater2->runAction(Sequence::create(DelayTime::create(0.5f), FadeIn::create(0.5f), CallFunc::create([=](){
                    
                    auto pFlash = Sprite::create("images/home/flash.png");
                    pFlash->setPosition(4, 8);
                    m_pWater2->addChild(pFlash);
                    pFlash->setOpacity(0);
                    pFlash->runAction(RepeatForever::create(Sequence::create(Spawn::create(RotateBy::create(1.0f, 180), FadeIn::create(1.0f), nullptr), FadeOut::create(0.6f), nullptr)));
                }), nullptr));
            }
        }
    
    }), Spawn::create(ScaleTo::create(0.2f, 1.0f), MoveBy::create(0.2f, Vec2(0, -25)), nullptr), nullptr));
 
}

void HomeLayer::onTouchDown(ToolSprite* toolSprite,Touch *pTouch)
{
    if (toolSprite->getTag() == eStartBtn_tag) {
        m_pTarget->setColor(Color3B(m_pTarget->getColor().r / 2, m_pTarget->getColor().g / 2, m_pTarget->getColor().b / 2));
    }
}

void HomeLayer::onTouchUp(ToolSprite* toolSprite, Touch *pTouch)
{
    if (toolSprite->getTag() == eStartBtn_tag) {
        m_pTarget->setColor(Color3B(m_pTarget->getColor().r * 2, m_pTarget->getColor().g * 2, m_pTarget->getColor().b * 2));
    }
    
}

void HomeLayer::onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch)
{
    switch (toolSprite->getTag())
    {
        case eStartBtn_tag:
        {
            auto pLockLayer = LockScreenLayer::create();
            this->addChild(pLockLayer, INT16_MAX);
            
            SoundPlayer::getInstance()->playEffect(playSound);
            
            if(kIAPManager->isShowAds())
            {
                AdsLoadingLayer::showLoading<AdsLoadingLayer>(false, nullptr, INT16_MAX);
                AdsLoadingLayer::loadingDoneCallback = []{
                    SceneManager::getInstance()->enterMakeConeStep01Scene();
                };
            }
            else
            {
                SceneManager::getInstance()->enterMakeConeStep01Scene();
            }
        }
            break;
            
        case eMoreBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            
            m_bIsStartKey = false;
            CFSystemFunction cf;
            cf.showMoreGame();
        }
            break;
            
        case eShopBtn_tag:
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            SceneManager::getInstance()->enterShopScene();
        }
            break;
    }
}


void HomeLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
    for (int i = 0; i < this->getParent()->getChildren().size(); i++) {
        if (this->getParent()->getChildren().at(i)->getLocalZOrder() > this->getLocalZOrder()) {
            m_bIsStartKey = false;
        }else{
            m_bIsStartKey = true;
        }
    }
    if (!m_bIsStartKey) {
        return;
    }
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            isReturn = true;
            
            Dialog* dialog = Dialog::create((void*)kDialogExit, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.78f, 0.4f);
            dialog->setContentText("Are you sure you want to exit this game?");
            dialog->setCallback(this);
            this->addChild(dialog, INT16_MAX);
        }
    }
}

void HomeLayer::onNegativeClick(void* type)
{
    isReturn = false;
}
void HomeLayer::onPositiveClick(void* type)
{
    isReturn = false;
    
    CFSystemFunction cf;
    
    cf.endSession();
    
    Director::getInstance()->end();
}

void HomeLayer::onEnter()
{
    Layer::onEnter();
    
    if (g_bBgMusicType) {
        g_bBgMusicType = false;
        SoundPlayer::getInstance()->stopBackGroundMusic();
        SoundPlayer::getInstance()->playBackGroundMusic(startBgMusick);
    }
    
    AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
    
    // show newBlast or request Fullscreen ads : cross
    static bool isFirst = true;
    
    if (isFirst) {
        
        isFirst = false;
        
        CFSystemFunction sys;
        sys.showNewsBlast(NewsModeLaunch);
        
    }else{
        if (kIAPManager->isShowAds() && g_bIsNeedAds) {
            AdLoadingLayerBase::showLoading<AdsLoadingLayer>(false);
        }
        g_bIsNeedAds = true;
    }
}

void HomeLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(HomeLayer::logoAnimation), 0.25f);
}

void HomeLayer::onExit()
{
    Layer::onExit();
}