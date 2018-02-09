//
//  BaseLayer.cpp
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-22.
//
//

#include "BaseLayer.h"
BaseLayer::BaseLayer()
{
    
}
BaseLayer::~BaseLayer()
{
    
}

void BaseLayer::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    //注册消息模式
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::showAdsLaod), "interstitialDidShow", NULL);
    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::closeAdsLaod), "interstitialDidmissed", NULL);
//    cocos2d::CCNotificationCenter::getInstance()->addObserver(this,callfuncO_selector(BaseLayer::purchaseFinish), "purchase_Finish", NULL);
    
}

void BaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}

void BaseLayer::onExit()
{
    Layer::onExit();
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "interstitialDidShow");
    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "interstitialDidmissed");
//    cocos2d::CCNotificationCenter::getInstance()->removeObserver(this, "purchase_Finish");
    
}

bool BaseLayer::init()
{
    bool isInit=false;
    
    do {
        CC_BREAK_IF(!Layer::init());
        CC_BREAK_IF(!initNecessary());
        isReturn = false;
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
    //        m_pLayerColor = LayerColor::create(Color4B(255, 0, 0, 0));
    //        m_pLayerColor->setContentSize(visibleSize);
    //        kAdapterScreen->setExactPosition(m_pLayerColor, 0, 0,kAnchorLB,kBorderLeft,kBorderBottom);
    
    auto e = EventListenerKeyboard::create();
    e->onKeyReleased = CC_CALLBACK_2(BaseLayer::onKeyReleasedCallBack, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    
    this->setKeypadEnabled(true);
    this->setKeyboardEnabled(true);
    
    //        auto eTouch = EventListenerTouchOneByOne::create();
    //        eTouch->setSwallowTouches(true);
    //        eTouch->onTouchBegan = CC_CALLBACK_2(BaseLayer::onScreenClick, this);
    //        this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eTouch,m_pLayerColor);
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


