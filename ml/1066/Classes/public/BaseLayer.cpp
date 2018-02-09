//
//  BaseLayer.cpp
//  FastFood
//
//  Created by huxixiang on 15/8/3.
//
//

#include "BaseLayer.h"
#include "IAPManager.h"
#include "AdsManager.h"
#include "AdsManager.h"

#define NEXTBTN_TAG 100
#define MARGIN      18

BaseLayer::BaseLayer():isReturn(false),m_pNextBtn(nullptr),m_bKeyEnabled(false),m_bNextBtnEnabled(true),m_bIsLockScreen(false),m_pLockLayer(nullptr)
{
    
}
BaseLayer::~BaseLayer()
{
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool BaseLayer::init()
{
    bool isInit = false;
    
    do {
        CC_BREAK_IF(!Layer::init());
        
        CC_BREAK_IF(!initNecessary());
        
        isInit = true;
        
    } while (0);
    
    return isInit;
}

bool BaseLayer::initNecessary()
{
    auto director = Director::getInstance();
    
    visibleOrigin = director->getVisibleOrigin();
    visibleSize = director->getVisibleSize();
    winSize = director->getWinSize();
    
    initUI();
    
    registEvent();
    
    return true;
}

void BaseLayer::initUI()
{
    m_pNextBtn = Button::create("images/ui/btn_next.png");
    kAdapterScreen->setExactPosition(m_pNextBtn, MARGIN - 123, MARGIN, Vec2(1, 1), kBorderRight, kBorderTop);
    m_pNextBtn->setTag(NEXTBTN_TAG);
    this->addChild(m_pNextBtn, INT16_MAX - 20);
    m_pNextBtn->addTouchEventListener(CC_CALLBACK_2(BaseLayer::onButtontouchCallback, this));
    m_pNextBtn->setVisible(false);
}

void BaseLayer::registEvent()
{
    this->setKeypadEnabled(true);
}


void BaseLayer::addLockScreenLayer(int zOrder)
{
    if (!m_bIsLockScreen) {
        m_bIsLockScreen = true;
        
        if(!m_pLockLayer){
            m_pLockLayer = LockScreenLayer::create();
            this->addChild(m_pLockLayer, zOrder);
        }
    }
}

void BaseLayer::removeLockScreenLayer()
{
    if (m_bIsLockScreen) {
        m_bIsLockScreen = false;
        
        if (m_pLockLayer) {
            m_pLockLayer->removeFromParentAndCleanup(true);
            m_pLockLayer = nullptr;
        }
    }
}

bool BaseLayer::isLockScreen()
{
    return m_bIsLockScreen;
}

LockScreenLayer* BaseLayer::getLockScreenLayer()
{
    return m_pLockLayer;
}

void BaseLayer::onButtontouchCallback(Ref *pSender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
    {
        m_pNextBtn->setColor(Color3B(m_pNextBtn->getColor().r / 2, m_pNextBtn->getColor().g / 2, m_pNextBtn->getColor().b / 2));
    }
    
    if (type == Widget::TouchEventType::ENDED)
    {
        m_pNextBtn->setTouchEnabled(false);
        m_pNextBtn->setColor(Color3B(m_pNextBtn->getColor().r * 2, m_pNextBtn->getColor().g * 2, m_pNextBtn->getColor().b * 2));
        
        SoundPlayer::getInstance()->playEffect(nextSound);
        onNextCallback();
    }
    
    if (type == Widget::TouchEventType::CANCELED)
    {
        m_pNextBtn->setColor(Color3B(m_pNextBtn->getColor().r * 2, m_pNextBtn->getColor().g * 2, m_pNextBtn->getColor().b * 2));
    }

    
}

Button* BaseLayer::getNextButton()
{
    return m_pNextBtn;
}

bool BaseLayer::getNextBtnVisible()
{
    return m_pNextBtn->isVisible();
}

void BaseLayer::setNextBtnIsVisible(bool isVisible, bool isNeedAction)
{
    if (isVisible)
    {
        if (isNeedAction)
        {
            m_pNextBtn->setTouchEnabled(false);
            m_pNextBtn->setVisible(true);
            m_pNextBtn->runAction(Sequence::create(EaseExponentialOut::create(MoveBy::create(1.0f, Vec2(-123, 0))), CallFunc::create([=](){
                
                m_pNextBtn->setTouchEnabled(m_bNextBtnEnabled);
//                m_pNextBtn->runAction(RepeatForever::create(Sequence::create(JumpBy::create(1.2f, Vec2::ZERO, 13, 3), DelayTime::create(2.0f), nullptr)));
            }), nullptr));
        }else{
            kAdapterScreen->setExactPosition(m_pNextBtn, MARGIN, MARGIN, Vec2(1, 1), kBorderRight, kBorderTop);
            m_pNextBtn->setVisible(true);
        }
        
    }else{
        if (isNeedAction) {
            m_pNextBtn->setTouchEnabled(false);
            m_pNextBtn->runAction(Sequence::create(EaseExponentialIn::create(MoveBy::create(0.4f, Vec2(123, 0))), CallFunc::create([=](){
                
                m_pNextBtn->setVisible(false);
            }), nullptr));
        }else{
            m_pNextBtn->setVisible(false);
            kAdapterScreen->setExactPosition(m_pNextBtn, MARGIN - 123, MARGIN, Vec2(1, 1), kBorderRight, kBorderTop);
        }
    }
}

bool BaseLayer::getNextBtnEnabled()
{
    return m_pNextBtn->isTouchEnabled();
}

void BaseLayer::setNextBtnIsEnabled(bool isEnabled)
{
    m_bNextBtnEnabled = isEnabled;
    m_pNextBtn->setTouchEnabled(isEnabled);
}

void BaseLayer::setKeyEnabled(bool enable)
{
    m_bKeyEnabled = enable;
}

void BaseLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event* event)
{
    if (m_bKeyEnabled)
        return;
    
    if(keyCode == EventKeyboard::KeyCode::KEY_BACK)
    {
        if(!isReturn)
        {
            SoundPlayer::getInstance()->playEffect(buttonSound);
            isReturn = true;
            
            Dialog* dialog = Dialog::create((void*)kDialogReturnHome, Dialog::DIALOG_TYPE_NEGATIVE, DialogDirection::HORIZONTAL, 0.8f, 0.45f);
            dialog->setContentText("Do you want to start over with a new Ice Cream?");
            dialog->setCallback(this);
            this->addChild(dialog, INT16_MAX);
        }
    }
}

void BaseLayer::onNegativeClick(void* type)
{
    isReturn = false;
}

void BaseLayer::onPositiveClick(void* type)
{
    isReturn = false;
    
    int lType = (uintptr_t)type;
    
    if (lType == kDialogReturnHome)
    {
        this->addLockScreenLayer(INT16_MAX);
        
        g_bEatSoundPlaying = false;
        g_bIsNeedAds = false;
        g_bIsFromShop = false;

        g_iIceCreamType = -1;
        g_iStickerIndex = -1;
        g_iSyrupIndex = -1;
        g_iIceCreamCount = 0;
        g_bBgMusicType = true;
        g_bIsFlag = false;
        
        if(kIAPManager->isShowAds())
        {
            AdsManager::getInstance()->showAds(ADS_TYPE::kTypeInterstitialAds);
        }
        SceneManager::getInstance()->popToRootScene();
    }
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
    
    if(kIAPManager->isShowAds())
        AdsManager::getInstance()->showAds(ADS_TYPE::kTypeBannerAds);
    else
        AdsManager::getInstance()->removeAds(ADS_TYPE::kTypeBannerAds);
}

void BaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}
