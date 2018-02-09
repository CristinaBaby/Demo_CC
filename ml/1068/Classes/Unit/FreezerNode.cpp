
#include "FreezerNode.h"
#include "AudioHelp.h"
#include "AnimationHelp.h"

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
    
    m_pBody = Sprite::create("content/make/snowcone/ice/freezer/freezer.png");
    this->addChild(m_pBody);
    
    
    m_pBodyDoor = Sprite::create("content/make/snowcone/ice/freezer/closed0.png");
    this->addChild(m_pBodyDoor,1);
    m_pBodyDoor->setAnchorPoint(Vec2(143.0f/m_pBodyDoor->getContentSize().width,0.5));
    m_pBodyDoor->setPosition(Vec2(143.0f-m_pBody->getAnchorPointInPoints().x, 0));
    
    m_pBodyDoorOpen = Sprite::create("content/make/snowcone/ice/freezer/open0.png");
    this->addChild(m_pBodyDoorOpen,1);
    m_pBodyDoorOpen->setAnchorPoint(Vec2(260.0f/m_pBodyDoorOpen->getContentSize().width,0.5));
    m_pBodyDoorOpen->setPosition(Vec2(143.0f-m_pBody->getAnchorPointInPoints().x, -30));
    
    m_bIsOpen = false;
    m_bTouchEnable = true;
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
        Rect rect = this->_getDoorTouchWorldRect();
        if (m_bIsOpen) {
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
    m_pBodyDoor->setRotation3D(Vec3(0, 0, 0));
    m_pBodyDoor->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0, -80, 0)),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pBodyDoor->setVisible(false);
                                                                 m_pBodyDoorOpen->setVisible(true);
                                                                 m_pBodyDoorOpen->setRotation3D(Vec3(0, 80, 0));
                                                                 m_pBodyDoorOpen->runAction(RotateBy::create(0.5, Vec3(0, -80, 0)));
                                                             }), NULL));
}

void FreezerNode::closeDoor()
{
    AudioHelp::getInstance()->playEffect("fridge_off.mp3");
    m_bIsOpen = false;
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0,80,0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pBodyDoor->setVisible(true);
                                                                     m_pBodyDoorOpen->setVisible(false);
                                                                     m_pBodyDoor->runAction(RotateBy::create(0.5, Vec3(0, 80, 0)));
                                                                 }), NULL));
}

void FreezerNode::doFreeze()
{
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                    MoveBy::create(0.1, Vec2(-4, -4)),
                                                    MoveBy::create(0.1, Vec2(2, 2)),
                                                    NULL)));
    m_pBodyDoor->runAction(AnimationHelp::createAnimate("content/make/snowcone/ice/freezer/closed", 0, 3,false,true,1.5));
    m_pBodyDoorOpen->setTexture("content/make/snowcone/ice/freezer/open.png");
}

void FreezerNode::stopFreeze()
{
    this->stopAllActions();
}