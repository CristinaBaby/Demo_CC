//
//  BaseLayer.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-22.
//
//

#include "BaseLayer.h"
BaseLayer::BaseLayer():m_bIsShowReturn(false),m_bIsQuitGame(false)
{
    
}
BaseLayer::~BaseLayer()
{
    
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //显示广告的回调
//    _eventDispatcher->addCustomEventListener(kAdNotifyInterstitialShown, CC_CALLBACK_1(BaseLayer::showAdsLaod, this));
//    _eventDispatcher->addCustomEventListener(kAdNotifyInterstitialDismissed, CC_CALLBACK_1(BaseLayer::closeAdsLaod, this));
   
}

void BaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void BaseLayer::onExit()
{
    Layer::onExit();
//    _eventDispatcher->removeCustomEventListeners(kAdNotifyInterstitialShown);
//    _eventDispatcher->removeCustomEventListeners(kAdNotifyInterstitialDismissed);
    
}

bool BaseLayer::init()
{
    bool isInit=false;
    
    do {
        CC_BREAK_IF(!Layer::init());
        CC_BREAK_IF(!initNecessary());
        
        isInit=true;
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
    m_pBackGroundLayer = Layer::create();
    m_pContentLayer = Layer::create();
    m_pUILayer = Layer::create();
    this->addChild(m_pBackGroundLayer, 0);
    this->addChild(m_pContentLayer, 5);
    this->addChild(m_pUILayer, 10);

}

void BaseLayer::registEvent()
{
    auto e = EventListenerKeyboard::create();
    e->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleasedCallBack, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
}

void BaseLayer::addToBackGroundLayer(Node *child, int localZOrder)
{
    m_pBackGroundLayer->addChild(child, localZOrder);
}

void BaseLayer::addToContentLayer(Node *child, int localZOrder)
{
    m_pContentLayer->addChild(child, localZOrder);
}

void BaseLayer::addToUILayer(Node *child, int localZOrder)
{
    m_pUILayer->addChild(child, localZOrder);
}


