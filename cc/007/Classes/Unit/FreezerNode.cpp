
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
    
    m_pBody = Sprite::create("content/make/freeze/refrigerator_1.png");
    this->addChild(m_pBody);
    
    m_pBodyFront = Sprite::create("content/make/freeze/refrigerator_1_on.png");
    this->addChild(m_pBodyFront,5);
    
    m_pBodyDoor = Sprite::create("content/make/freeze/refrigerator_3.png");
    this->addChild(m_pBodyDoor,5);
    
    m_pBodyDoorOpen = Sprite::create("content/make/freeze/refrigerator_2.png");
    this->addChild(m_pBodyDoorOpen,5);
    m_pBodyDoorOpen->setAnchorPoint(Vec2(0.5, 540.0/m_pBodyDoorOpen->getContentSize().height));
    m_pBodyDoorOpen->setPosition(Vec2(0, 540.0-m_pBodyDoorOpen->getContentSize().height/2));
    
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
    AudioHelp::getInstance()->playEffect("open_close_door.mp3");
    m_bIsOpen = true;
    m_pBodyDoor->setVisible(false);
    m_pBodyDoorOpen->setRotation3D(Vec3(-70, 0, 0));
    m_pBodyDoorOpen->setVisible(true);
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(70, 0, 0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                 }), NULL));
}

void FreezerNode::closeDoor()
{
    AudioHelp::getInstance()->playEffect("open_close_door.mp3");
    m_bIsOpen = false;
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(-70,0,0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pBodyDoor->setVisible(true);
                                                                     m_pBodyDoorOpen->setVisible(false);
                                                                 }), NULL));
}

void FreezerNode::doFreeze()
{
    AudioHelp::getInstance()->playLoopEffect("freeze_running.mp3");
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 0)),
                                                    MoveBy::create(0.1, Vec2(-4, 0)),
                                                    MoveBy::create(0.1, Vec2(2, 0)),
                                                    NULL)));
}

void FreezerNode::stopFreeze()
{
    AudioHelp::getInstance()->stopLoopEffect();
    AudioHelp::getInstance()->playEffect("ding.mp3");
    this->stopAllActions();
}