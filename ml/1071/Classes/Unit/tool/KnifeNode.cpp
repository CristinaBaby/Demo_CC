
#include "KnifeNode.h"
#include "Global.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"
#include "CircleMove.h"
#include "AudioHelp.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;


USING_NS_CC;

KnifeNode::KnifeNode()
{
    m_pBlade = nullptr;
    m_pHandle = nullptr;
    m_pClippingNode = nullptr;
}

KnifeNode::~KnifeNode()
{
    
}




bool KnifeNode::init()
{
    if (!Node::init()) {
        return false;
    }
    Node* pCocosNode = CSLoader::createNode("KnifeNode.csb");
    
    Sprite* pStencil = Sprite::create("content/make/twist/make2/knife_mask.png");
    Node* pCocosMask = pCocosNode->getChildByName("knife_mask");
    m_pClippingNode = ClippingNode::create(pStencil);
    m_pClippingNode->setPosition(pCocosMask->getPosition());
    m_pClippingNode->setAlphaThreshold(1);
    m_pClippingNode->setInverted(true);
    this->addChild(m_pClippingNode,1);
    
    m_pBlade = Sprite::create("content/make/twist/make2/knife2.png");
    Node* pCocosBlade = pCocosMask->getChildByName("knife2");
    m_pBlade->setAnchorPoint(pCocosBlade->getAnchorPoint());
    m_pBlade->setPosition(pCocosBlade->getPosition()-pStencil->getAnchorPointInPoints());
    m_pClippingNode->addChild(m_pBlade);
    
    m_pBladeBack = Sprite::create("content/make/twist/make2/knife2.png");
    m_pBladeBack->setAnchorPoint(pCocosBlade->getAnchorPoint());
    m_pBladeBack->setPosition(this->convertToNodeSpace(m_pClippingNode->convertToWorldSpace(m_pBlade->getPosition())));
    this->addChild(m_pBladeBack);
    
    m_pHandle = Sprite::create("content/make/twist/make2/knife1.png");
    this->addChild(m_pHandle,5);
    Node* pCocosHandle = pCocosNode->getChildByName("knife1");
    m_pHandle->setAnchorPoint(pCocosHandle->getAnchorPoint());
    m_pHandle->setPosition(pCocosHandle->getPosition());
    m_pHandle->setRotation(-90);
    return true;
}


void KnifeNode::cut(Vec2 center,float raduis)
{
    startWork();
    this->runAction(Sequence::create(CircleMove::create(1, center, raduis, 360),
                                     CallFunc::create([=](){
        stopWork();
    }), NULL));
}

void KnifeNode::startWork()
{
//    m_pBladeBack->setVisible(false);
    m_pBlade->runAction(RepeatForever::create(RotateBy::create(1, 360)));
//    m_pHandle->runAction(RotateBy::create(1, 360));
//    m_pClippingNode->runAction(RotateBy::create(1, 360));
    this->runAction(RotateBy::create(1, 360));
}

void KnifeNode::stopWork()
{
    m_pBladeBack->setVisible(true);
    m_pBlade->stopAllActions();
}