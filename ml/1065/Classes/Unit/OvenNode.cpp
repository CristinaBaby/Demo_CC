
#include "OvenNode.h"
#include "AnimationHelp.h"

OvenNode::OvenNode()
{
    m_pDoorOpen = nullptr;
    m_bIsWork = false;
    m_bIsOpen = false;
    m_bCanTouch = true;
    m_bBaked = false;
}

OvenNode::~OvenNode()
{
    
}

bool OvenNode::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_eState = State::eStateDoorClose;
    
    m_pBody = Sprite::create("content/make/bake/oven.png");
    this->addChild(m_pBody);
    
    m_pDoor = Sprite::create("content/make/bake/door0.png");
    this->addChild(m_pDoor,9);
    m_pDoor->setPosition(Vec2(-190, -155));
    
    m_pPotLeft = Sprite::create("content/make/bake/pot0.png");
    this->addChild(m_pPotLeft);    
    m_pPotLeft->setPosition(Vec2(-200, 200));
    
    m_pPotRight = Sprite::create("content/make/bake/pot2.png");
    this->addChild(m_pPotRight);
    m_pPotRight->setPosition(Vec2(160, 230));
    
    m_pSwitch = Button::create("content/make/bake/off.png","content/make/bake/off.png","content/make/bake/off.png");
    this->addChild(m_pSwitch,10);
    m_pSwitch->setPosition(Vec2(40, 58));
    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(OvenNode::onSwitchCallback, this));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(OvenNode::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(OvenNode::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(OvenNode::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(OvenNode::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void OvenNode::onSwitchCallback(Ref*,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        m_pSwitch->setTouchEnabled(false);
        m_bIsWork = !m_bIsWork;
        if (m_bIsWork) {
            m_eState = State::eStateTurnOn;
            m_pSwitch->loadTextures("content/make/bake/on.png", "content/make/bake/on.png","content/make/bake/on.png");
            m_pDoor->setTexture("content/make/bake/door0_1.png");
            if (onOvenStateCallback) {
                onOvenStateCallback(State::eStateTurnOn);
            }
        }else{
            m_eState = State::eStateTurnOff;
            Node* pSmoke = this->getChildByName("smoke");
            if(pSmoke){
                pSmoke->removeFromParent();
            }
            m_pSwitch->loadTextures("content/make/bake/off.png", "content/make/bake/off.png","content/make/bake/off.png");
            m_pDoor->setTexture("content/make/bake/door0.png");
            if (onOvenStateCallback) {
                onOvenStateCallback(State::eStateTurnOff);
            }
        }
    }
}

void OvenNode::openDoor()
{
    m_bCanTouch = false;
    if (!m_pDoorOpen) {
        m_pDoorOpen = Sprite::create("content/make/bake/door2.png");
        this->addChild(m_pDoorOpen,9);
        m_pDoorOpen->setPosition(Vec2(-250, -500));
        m_pDoorOpen->setVisible(false);
    }
    m_pDoor->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/bake/door", 0, 1,false),
                                          CallFunc::create([=]()
                                                           {
                                                               m_pDoor->setVisible(false);
                                                               m_pDoorOpen->setVisible(true);
                                                               m_bCanTouch = true;
                                                               if (m_bBaked) {
                                                                   m_eState = State::eStateDoorOpenBaked;
                                                               }else{
                                                                   m_eState = State::eStateDoorOpen;
                                                               }
                                                               if (onOvenStateCallback) {
                                                                   onOvenStateCallback(m_eState);
                                                               }
                                                           }), NULL));
}

void OvenNode::closeDoor()
{
    m_bCanTouch = false;
    if (m_pDoorOpen) {
        m_pDoorOpen->setVisible(false);
    }
    m_pDoor->setVisible(true);
    m_pDoor->runAction(Sequence::create(AnimationHelp::createAnimate("content/make/bake/door", 1, 0,false),
                                        CallFunc::create([=]()
                                                         {
                                                             m_bCanTouch = true;
                                                             if (m_bBaked) {
                                                                 m_eState = State::eStateDoorCloseBaked;
                                                             }else{
                                                                 m_eState = State::eStateDoorClose;
                                                             }
                                                             if (onOvenStateCallback) {
                                                                 onOvenStateCallback(m_eState);
                                                             }
                                                         }), NULL));
}


bool OvenNode::onTouchBegan(Touch *touch, Event *unused_event)
{
    return true;
}

void OvenNode::onTouchMoved(Touch *touch, Event *unused_event)
{
    
}

void OvenNode::onTouchEnded(Touch *touch, Event *unused_event)
{
    if (m_bIsWork) {
        return;
    }
    if (!m_bCanTouch) {
        return;
    }
    Rect rect = Rect(75, 140, 400, 330);
    rect.origin = m_pBody->convertToWorldSpace(rect.origin);
    Vec2 location = touch->getLocation();
    if (!rect.containsPoint(location)) {
        return;
    }
    m_bIsOpen=!m_bIsOpen;
    if (m_bIsOpen) {
        openDoor();
    }else{
        closeDoor();
    }
}

void OvenNode::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}