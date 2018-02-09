
#include "KettleNode.h"
#include "Global.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"


using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

KettleNode::KettleNode()
{
    m_pClippingNode = nullptr;
    m_pGridNode = nullptr;
    m_pWater = nullptr;
}

KettleNode::~KettleNode()
{
    
}


KettleNode* KettleNode::create(Node* pParent)
{
    KettleNode* node = new KettleNode();
    if (node && node->init(pParent))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_RELEASE(node);
    return NULL;
    
}


bool KettleNode::init(Node* pParent)
{
    ContainerNode::init(pParent);
    return true;
}

void KettleNode::addStraw(Node* pNode)
{
    pNode->setName("straw_clipping");
    m_pParent->addChild(pNode,10);
}

void KettleNode::addWater(std::string water,std::string mask,Vec2 pos)
{
    m_pathWater = water;
    if (!m_pClippingNode) {
        m_pClippingNode = ClippingNode::create(Sprite::create(mask));
        m_pParent->addChild(m_pClippingNode,1);
        m_allChildren.pushBack(m_pClippingNode);
    }
//    m_pClippingNode->setStencil(Sprite::create(mask));
    m_pClippingNode->setAlphaThreshold(0.5);
    m_pClippingNode->setPosition(pos);
    
    if (!m_pGridNode) {
        m_pGridNode = NodeGrid::create();
        m_pClippingNode->addChild(m_pGridNode);
    }
    if (m_pWater) {
        m_pWater->removeFromParent();
    }
    m_pWater = Sprite::create(water);
    m_pGridNode->addChild(m_pWater);
    
    m_pWater->setAnchorPoint(Vec2::ZERO);
    m_pWater->setPosition(-Vec2(m_pWater->getContentSize().width*0.5,m_pWater->getContentSize().height*0.5));
}

void KettleNode::addPourWater(std::string water){
    m_pathPourWater = std::string(water);
}

void KettleNode::waveWater(int wave,float dt)
{
    if (m_pGridNode) {
        if (wave==0) {
            m_pGridNode->runAction(RepeatForever::create(Sequence::create(Liquid::create(1, Size(10, 10), 2, 3),
                                                                          DelayTime::create(0.1), NULL)));
        }else{
            m_pGridNode->runAction(Liquid::create(dt, Size(10, 10), wave, 5));
        }
    }
    
}

void KettleNode::pourWater(float dt)
{
    waveWater(8);
    if (m_pWater) {
        if(m_pathPourWater!=""){
            m_pWater->setTexture(m_pathPourWater);
        }
        m_pWater->runAction(Sequence::create(ScaleTo::create(dt, 0.3,1),
                                             DelayTime::create(0.5),
                                             CallFunc::create([=](){
            m_pGridNode->stopAllActions();
            if(m_pathWater!=""){
                m_pWater->setTexture(m_pathWater);
            }
            m_pWater->setScale(1, 0.4);
        }), NULL));
    }
}
void KettleNode::pourWaterOnce(int percent,float dt)
{
    waveWater(8);
    if (m_pWater) {
        if(m_pathPourWater!=""){
            m_pWater->setTexture(m_pathPourWater);
        }
        m_pWater->runAction(Sequence::create(ScaleTo::create(dt, 1.0-percent*0.01,1),
                                             DelayTime::create(0.5),
                                             CallFunc::create([=](){
            m_pGridNode->stopAllActions();
            if(m_pathWater!=""){
                m_pWater->setTexture(m_pathWater);
            }
            m_pWater->setScale(1, 1.0-percent*0.01);
        }), NULL));
    }
    
}
void KettleNode::updateWaterOnce(int percent,float dt)
{
    waveWater(8);
    if (m_pWater) {
        m_pWater->runAction(Sequence::create(ScaleTo::create(dt, 1,1.0-percent*0.01),
                                             DelayTime::create(0.5),
                                             CallFunc::create([=](){
            m_pGridNode->stopAllActions();
        }), NULL));
    }
    
}

void KettleNode::startDrink()
{
    waveWater(8);
    if (m_pWater) {
        if(m_pathPourWater!=""){
            m_pWater->setTexture(m_pathPourWater);
        }
        scheduleUpdate();
    }
    AudioHelp::getInstance()->playDrinkEffect();
}
void KettleNode::stopDrink()
{
    unscheduleUpdate();
    m_pGridNode->stopAllActions();
    if(m_pathWater!=""){
        m_pWater->setTexture(m_pathWater);
    }
    
    AudioHelp::getInstance()->stopDrinkEffect();
}

void KettleNode::update(float){
    
    if(m_pWater->getScaleY()-0.002<=0){
        m_pWater->setScale(1, 0);
        stopDrink();
        return;
    }
    m_pWater->setScale(1, m_pWater->getScaleY()-0.002);
    Node* pStrawClip = m_pParent->getChildByName("straw_clipping");
    if(pStrawClip){
        float delt = -m_pWater->getContentSize().height*0.75*(1-m_pWater->getScaleY())-pStrawClip->getPositionY();
        pStrawClip->setPosition(Vec2(0,-m_pWater->getContentSize().height*0.75*(1-m_pWater->getScaleY())));
        Node* pStaw = pStrawClip->getChildByName("straw");
        if(pStaw){
            pStaw->setPosition(pStaw->getPosition()+Vec2(0,-delt));
//            Vec2 pos = pStaw->getPosition();
//            pStaw->setPosition(pos+Vec2(0,m_pWater->getContentSize().height*(m_pWater->getScaleY())));
        }
    }
}