
#include "FreezerNode.h"
#include "AudioHelp.h"

FreezerNode::FreezerNode()
{
    onDoorCallback = nullptr;
}

FreezerNode::~FreezerNode()
{
    
}
bool FreezerNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_pLightOut = Sprite::create("content/make/ice/freezer4.png");
    this->addChild(m_pLightOut);
    
    m_pBody = Sprite::create("content/make/ice/freezer0.png");
    this->addChild(m_pBody);
    
    m_pLightIn = Sprite::create("content/make/ice/light.png");
    this->addChild(m_pLightIn);
    
    m_pBodyFront = Sprite::create("content/make/ice/freezer1.png");
    this->addChild(m_pBodyFront);
    
    m_pBodyDoor = Sprite::create("content/make/ice/freezer2.png");
    this->addChild(m_pBodyDoor,1);
    
    m_pBodyDoorOpen = Sprite::create("content/make/ice/freezer3.png");
    this->addChild(m_pBodyDoorOpen,1);
    m_pBodyDoorOpen->setAnchorPoint(Vec2(0.5, 0));
    m_pBodyDoorOpen->setPosition(Vec2(0, 236));
    
    m_bIsOpen = false;
    m_bTouchEnable = true;
    m_pLightIn->setOpacity(0);
    m_pLightOut->setOpacity(0);
    m_pBodyDoorOpen->setVisible(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        return true;
    };
    listener->onTouchEnded = [=](Touch* pTouch,Event* event){
        if (!m_bTouchEnable) {
            return;
        }
        Vec2 location = pTouch->getLocation();
        Rect rect = m_pBody->getBoundingBox();
        rect.origin = this->convertToWorldSpace(rect.origin);
        if (m_bIsOpen) {
            rect.origin.y += rect.size.height;
            if (rect.containsPoint(location)) {
                closeDoor();
                if (onDoorCallback) {
                    onDoorCallback(this);
                }
            }
        }else{
            if (rect.containsPoint(location)) {
                openDoor();
                if (onDoorCallback) {
                    onDoorCallback(this);
                }
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void FreezerNode::openDoor()
{
    AudioHelp::getInstance()->playEffect("fridge_on.mp3");
    m_bIsOpen = true;
    m_pBodyDoor->setVisible(false);
    m_pBodyDoorOpen->setRotation3D(Vec3(-70, 0, 0));
    m_pBodyDoorOpen->setVisible(true);
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(70, 0, 0)),
                                                CallFunc::create([=]()
                                                                 {
//                                                                     m_pLightIn->runAction(FadeIn::create(0.5));
                                                                     m_pLightIn->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2, 255),
                                                                                                                                  EaseBackOut::create(FadeTo::create(1, 180)),
                                                                                                                                  DelayTime::create(0.1), NULL)));
                                                                 }), NULL));
}

void FreezerNode::closeDoor()
{
    AudioHelp::getInstance()->playEffect("fridge_off.mp3");
    m_bIsOpen = false;
    m_pLightOut->stopAllActions();
    m_pLightOut->setOpacity(0);
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(-70,0,0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pBodyDoor->setVisible(true);
                                                                     m_pBodyDoorOpen->setVisible(false);
                                                                 }), NULL));
}

void FreezerNode::doFreeze()
{
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                    MoveBy::create(0.1, Vec2(-4, -4)),
                                                    MoveBy::create(0.1, Vec2(2, 2)),
                                                    NULL)));
    
    m_pLightOut->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2, 255),
                                                                 EaseBackOut::create(FadeTo::create(1, 180)),
                                                                 DelayTime::create(0.1), NULL)));
}

void FreezerNode::stopFreeze()
{
    this->stopAllActions();
    m_pLightOut->stopAllActions();
    m_pLightOut->setOpacity(0);
}