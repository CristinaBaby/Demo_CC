
#include "ContainerNode.h"
#include "Global.h"
#include "AudioHelp.h"


USING_NS_CC;

ContainerNode::ContainerNode()
{
    m_pFood = nullptr;
    m_pClippingNode = nullptr;
}

ContainerNode::~ContainerNode()
{
    
}


ContainerNode* ContainerNode::create(Node* pParent)
{
    ContainerNode* node = new ContainerNode();
    if (node && node->init(pParent))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_RELEASE(node);
    return NULL;
    
}


bool ContainerNode::init(Node* pParent)
{
    m_pParent = pParent;
    m_pParent->addChild(this);
    return true;
}


void ContainerNode::addBody(std::string body)
{
    Sprite* pBody = Sprite::create(body);
    m_pParent->addChild(pBody,0);
    pBody->setName("body");
    m_allChildren.pushBack(pBody);
}

void ContainerNode::addFront(std::string front,std::string name,Vec2 pos)
{
    Sprite* pFront = Sprite::create(front);
    m_pParent->addChild(pFront,10);
    pFront->setName(name);
    pFront->setPosition(pos);
    m_allChildren.pushBack(pFront);
}

void ContainerNode::addShadow(std::string shadow,Vec2 pos)
{
    Sprite* pShadow = Sprite::create(shadow);
    pShadow->setPosition(pos);
    m_pParent->addChild(pShadow,-1);
    pShadow->setName("shadow");
    m_allChildren.pushBack(pShadow);
}
void ContainerNode::showShadow(bool show)
{
    Node* pShadow = m_pParent->getChildByName("shadow");
    if (pShadow) {
        pShadow->setVisible(show);
    }
}

void ContainerNode::addFood(std::string food,std::string mask,Vec2 pos)
{
    if (!m_pClippingNode) {
        if (mask=="") {
            m_pClippingNode = ClippingNode::create(Sprite::create(food));
        }else{
            m_pClippingNode = ClippingNode::create(Sprite::create(mask));
        }
        m_pParent->addChild(m_pClippingNode,1);
        m_allChildren.pushBack(m_pClippingNode);
    }
    m_pClippingNode->setAlphaThreshold(0.5);
    if (m_pFood) {
        m_pFood->removeFromParent();
    }
    m_pFood = Sprite::create(food);
    m_pClippingNode->addChild(m_pFood,1);
    m_pFood->setPosition(pos);
}

void ContainerNode::addFoodShadow(std::string shadow,Vec2 pos)
{
    Sprite* pShadow = Sprite::create(shadow);
    pShadow->setPosition(pos);
    m_pParent->addChild(pShadow);
    pShadow->setName("food_shadow");
    m_allChildren.pushBack(pShadow);
}

void ContainerNode::pourFood(bool left,float dt)
{
    if (m_pFood) {
        Vec2 pos = Vec2((left?-1:1)*m_pFood->getContentSize().width*0.3, -m_pFood->getContentSize().height*0.2);
        m_pFood->runAction(Sequence::create(Spawn::create(ScaleTo::create(dt, 0.3),
                                                          MoveBy::create(dt, pos), NULL),
                                            FadeOut::create(0.3), NULL));
    }
    Node* pShadow = m_pParent->getChildByName("food_shadow");
    if (pShadow) {
        Vec2 pos = Vec2((left?-1:1)*m_pFood->getContentSize().width*0.3, -m_pFood->getContentSize().height*0.2);
        pShadow->runAction(Sequence::create(Spawn::create(ScaleTo::create(dt, 0.3),
                                                          MoveBy::create(dt, pos), NULL),
                                            FadeOut::create(0.3), NULL));
    }
}