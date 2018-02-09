
#include "EatLayer.h"
#include "AudioHelp.h"

EatLayer::EatLayer()
{
    onEatBeginCallback = nullptr;
    onEatEndCallback = nullptr;
    onEatMoveCallback = nullptr;
}

EatLayer::~EatLayer()
{
    
}
bool EatLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(EatLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(EatLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(EatLayer::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(EatLayer::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


bool EatLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    Vec2 position = pTouch->getLocation();
    if (onEatBeginCallback) {
        bool enable = false;
        if (m_DrinkRect.containsPoint(position)) {
            enable = true;
        }
        onEatBeginCallback(position,enable);
    }
    return true;
}
void EatLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Vec2 position = pTouch->getLocation();
    if (onEatMoveCallback) {
        onEatMoveCallback(position);
    }
    
}
void EatLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Vec2 position = pTouch->getLocation();
//    AudioHelp::getInstance()->playEffect("basic/eat.mp3");
//    ParticleSystemQuad* pParticle = ParticleSystemQuad::create("particle/biting.plist");
//    this->addChild(pParticle);
//    pParticle->setPosition(position);
    if (onEatEndCallback) {
        onEatEndCallback(position);
    }
}
void EatLayer::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    Vec2 position = pTouch->getLocation();
    if (onEatEndCallback) {
        onEatEndCallback(position);
    }
    
}