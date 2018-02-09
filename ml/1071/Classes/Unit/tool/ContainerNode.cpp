
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


void ContainerNode::addFront(std::string front)
{
    Sprite* pFront = Sprite::create(front);
    m_pParent->addChild(pFront,5);
    pFront->setName("front");
}

void ContainerNode::addShadow(std::string shadow,Vec2 pos)
{
    Sprite* pShadow = Sprite::create(shadow);
    pShadow->setPosition(pos);
    m_pParent->addChild(pShadow,-1);
    pShadow->setName("shadow");
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
    }
    m_pClippingNode->setAlphaThreshold(0.5);
//    if (mask=="") {
//        m_pClippingNode->setStencil(Sprite::create(food));
//    }else{
//        m_pClippingNode->setStencil(Sprite::create(mask));
//    }
    if (!m_pFood) {
        m_pFood = Sprite::create(food);
        m_pClippingNode->addChild(m_pFood,1);
    }else{
        m_pFood->setTexture(food);
    }
    m_pFood->setPosition(pos);
}

void ContainerNode::addFoodShadow(std::string shadow,Vec2 pos)
{
    Sprite* pShadow = Sprite::create(shadow);
    pShadow->setPosition(pos);
    m_pParent->addChild(pShadow);
    pShadow->setName("food_shadow");
}

void ContainerNode::pourFood(bool left)
{
    if (m_pFood) {
        Vec2 pos = Vec2((left?-1:1)*m_pFood->getContentSize().width*0.3, -m_pFood->getContentSize().height*0.2);
        m_pFood->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.3),
                                                          MoveBy::create(1, pos), NULL),
                                            FadeOut::create(0.3), NULL));
    }
    Node* pShadow = m_pParent->getChildByName("food_shadow");
    if (pShadow) {
        Vec2 pos = Vec2((left?-1:1)*m_pFood->getContentSize().width*0.3, -m_pFood->getContentSize().height*0.2);
        pShadow->runAction(Sequence::create(Spawn::create(ScaleTo::create(1, 0.3),
                                                          MoveBy::create(1, pos), NULL),
                                            FadeOut::create(0.3), NULL));
    }
}