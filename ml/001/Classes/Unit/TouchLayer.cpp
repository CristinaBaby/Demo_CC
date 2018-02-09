
#include "TouchLayer.h"

TouchLayer::TouchLayer()
{
    onTouchBegan = nullptr;
    onTouchEnded = nullptr;
    onTouchMoved = nullptr;
}

TouchLayer::~TouchLayer()
{
    
}

bool TouchLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(TouchLayer::TouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchLayer::TouchMove, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchLayer::TouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TouchLayer::TouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    m_bEnable = true;
    return true;
}
bool TouchLayer::TouchBegan(Touch *pTouch, Event *pEvent)
{
    if(!m_bEnable) {
        return false;
    }
    if (onTouchBegan) {
        onTouchBegan(pTouch,pEvent);
    }
    return true;
}
void TouchLayer::TouchMove(Touch *pTouch, Event *pEvent)
{
    if (onTouchMoved) {
        onTouchMoved(pTouch,pEvent);
    }
}
void TouchLayer::TouchEnded(Touch *pTouch, Event *pEvent)
{
    
    if (onTouchEnded) {
        onTouchEnded(pTouch,pEvent);
    }
}