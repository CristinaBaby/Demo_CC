//
//  CFLayer.cpp
//  KidsFramework_CocosV3.2
//
//  Created by zhangguangzong1 on 9/1/14.
//
//

#include "CFLayer.h"

bool CFLayer::init()
{
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!Layer::init());
        
        m_fnBackFunc = nullptr;
//        m_pLockLayer = nullptr;
        
        m_pBgLayer = Layer::create();
        m_pBgLayer->setPosition(Vec2::ZERO);
        this->addChild(m_pBgLayer,kBackground);
        
        m_pContentLayer = Layer::create();
        m_pContentLayer->setPosition(Vec2::ZERO);
        this->addChild(m_pContentLayer,kContent);
        
        m_pUILayer = Layer::create();
        m_pUILayer->setPosition(Vec2::ZERO);
        this->addChild(m_pUILayer,kUI);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

bool CFLayer::onBackKeyDown()
{
    if(m_fnBackFunc)
        return m_fnBackFunc();
    return false;
}

void CFLayer::onEnter()
{
//    KeypadDispatchCenter::getInstance()->addDelegate(this);
    Layer::onEnter();
}

void CFLayer::onExit()
{
    Layer::onExit();
//    KeypadDispatchCenter::getInstance()->removeDelegate(this);
}

void CFLayer::addToBgLayer(Node* node,int zorder,int tag)
{
    m_pBgLayer->addChild(node,zorder,tag);
}

void CFLayer::addToContentLayer(Node* node,int zorder,int tag)
{
    m_pContentLayer->addChild(node,zorder,tag);
}

void CFLayer::addToUILayer(Node* node,int zorder,int tag)
{
    m_pUILayer->addChild(node,zorder,tag);
}


//void CFLayer::addLockScreenLayer()
//{
//    if(!m_pLockLayer)
//    {
//        m_pLockLayer = LockScreenLayer::create();
//        
//        
//    }
//    else
//    {
//    
//        log("已经lock");
//    }
//    this->addToUILayer(m_pUILayer,250);
//}
//
//void CFLayer::removeLockScreenLayer()
//{
//    if(!m_pLockLayer)
//    {
//        
//        log("已经remove");
//        
//    }
//    else
//    {
//        m_pLockLayer->removeFromParent();
//    }
//   
//    
//}
