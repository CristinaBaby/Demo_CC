
#include "OvenNode.h"
#include "AnimationHelp.h"
#include "AudioHelp.h"
#include "cocostudio/CocoStudio.h"

OvenNode::OvenNode()
{
    m_pDoorOpen = nullptr;
    m_bIsWork = false;
    m_bIsOpen = false;
    m_bCanTouch = false;
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
    
    Node* pOven = CSLoader::createNode("res/OvenNode.csb");
    this->addChild(pOven);
    ui::Helper::doLayout(pOven);
    m_pOven = pOven;
    
    m_pBody = dynamic_cast<Sprite*>(pOven->getChildByName("oven"));
    m_pDoor = dynamic_cast<Sprite*>(pOven->getChildByName("oven_door_close"));
    m_pSwitch = dynamic_cast<Button*>(pOven->getChildByName("switch1"));
    m_pDoorOpen = dynamic_cast<Sprite*>(pOven->getChildByName("oven_door_open"));
    m_pPoint = dynamic_cast<Sprite*>(pOven->getChildByName("switch3"));
    m_pPoint->setVisible(false);
    
    m_pDoor->setLocalZOrder(10);
    m_pDoorOpen->setLocalZOrder(10);
    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(OvenNode::onSwitchCallback, this));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(OvenNode::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(OvenNode::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(OvenNode::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(OvenNode::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    reset();
    return true;
}

void OvenNode::putinFood(Node* pNode)
{
    m_pOven->addChild(pNode,1);
}
void OvenNode::onSwitchCallback(Ref*,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::ENDED) {
        m_pSwitch->setTouchEnabled(false);
        m_bIsWork = !m_bIsWork;
        if (m_bIsWork) {
            m_eState = State::eStateTurnOn;
            m_pSwitch->loadTextures("res/oven/switch2.png", "res/oven/switch2.png","res/oven/switch2.png");
            m_pDoor->setTexture("res/oven/door0_1.png");
            AudioHelp::getInstance()->playLoopEffect("baking.mp3");
            m_pDoor->runAction(RepeatForever::create(AnimationHelp::createAnimate("res/oven/oven_door_close", 1, 2,false,true,1)));
            
            if (onOvenStateCallback) {
                onOvenStateCallback(State::eStateTurnOn);
            }
        }else{
            m_eState = State::eStateTurnOff;
            m_pSwitch->loadTextures("res/oven/switch1.png", "res/oven/switch1.png","res/oven/switch1.png");
            m_pDoor->setTexture("res/oven/door0.png");
            if (onOvenStateCallback) {
                onOvenStateCallback(State::eStateTurnOff);
            }
        }
    }
}

void OvenNode::openDoor()
{
    m_pDoor->setVisible(false);
    m_pDoorOpen->setVisible(true);
    
    if (m_bBaked) {
        m_eState = State::eStateDoorOpenBaked;
    }else{
        m_eState = State::eStateDoorOpen;
    }
    if (onOvenStateCallback) {
        onOvenStateCallback(m_eState);
    }
}

void OvenNode::closeDoor()
{
    if (m_pDoorOpen) {
        m_pDoorOpen->setVisible(false);
    }
    m_pDoor->setVisible(true);
    
    if (m_bBaked) {
        m_eState = State::eStateDoorCloseBaked;
    }else{
        m_eState = State::eStateDoorClose;
    }
    if (onOvenStateCallback) {
        onOvenStateCallback(m_eState);
    }
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
    Rect rect = Rect(75, 243, 560, 430);
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