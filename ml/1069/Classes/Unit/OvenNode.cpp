
#include "OvenNode.h"
#include "AudioHelp.h"
#include "AnimationHelp.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
OvenNode::OvenNode()
{
    onDoorCallback = nullptr;
    onBakeCallback = nullptr;
    m_bBaked = false;
    m_bIsBaking = false;
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
    
    Node* pOVenNode = CSLoader::createNode("res/OvenNode.csb");
    ui::Helper::doLayout(pOVenNode);
    
    Node* pBody = pOVenNode->getChildByName("oven_1");
    Node* pBodyDoor = pOVenNode->getChildByName("oven_2");
    Node* BodyDoorOpen = pOVenNode->getChildByName("oven_4");
    Node* pSwitch = pOVenNode->getChildByName("oven_button1");
    
    m_pBody = Sprite::create("content/make/gummy/oven/oven_1.png");
    this->addChild(m_pBody);
    m_pBody->setPosition(pBody->getPosition());
    
    
    m_pBodyDoor = Sprite::create("content/make/gummy/oven/oven_2.png");
    this->addChild(m_pBodyDoor,1);
    m_pBodyDoor->setAnchorPoint(pBodyDoor->getAnchorPoint());
    m_pBodyDoor->setPosition(pBodyDoor->getPosition());
    
    m_pBodyDoorOpen = Sprite::create("content/make/gummy/oven/oven_4.png");
    this->addChild(m_pBodyDoorOpen,1);
    m_pBodyDoorOpen->setAnchorPoint(BodyDoorOpen->getAnchorPoint());
    m_pBodyDoorOpen->setPosition(BodyDoorOpen->getPosition());
    
    m_pSwitch = Button::create("content/make/gummy/oven/oven_button1.png","content/make/gummy/oven/oven_button1.png");
    this->addChild(m_pSwitch);
    m_pSwitch->setAnchorPoint(pSwitch->getAnchorPoint());
    m_pSwitch->setPosition(pSwitch->getPosition());
    m_pSwitch->setTouchEnabled(false);
    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(OvenNode::_onButtonCallbak, this));
    
    m_bIsOpen = false;
    m_bTouchEnable = false;
    m_pBodyDoorOpen->setVisible(false);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [=](Touch* pTouch,Event* event){
        return m_bTouchEnable;
    };
    listener->onTouchEnded = [=](Touch* pTouch,Event* event){
        if (!m_bTouchEnable) {
            return;
        }
        Vec2 location = pTouch->getLocation();
        Rect rect = this->getDoorTouchWorldRect();
        if (m_bIsOpen) {
            if (rect.containsPoint(location)) {
                closeDoor();
            }
        }else{
            if (rect.containsPoint(location)) {
                openDoor();
            }
        }
        this->runAction(Sequence::create(DelayTime::create(1),
                                         CallFunc::create([=]() {
            if (onDoorCallback) {
                onDoorCallback(this);
            }
        }), NULL));
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void OvenNode::openDoor()
{
    m_bTouchEnable = false;
    AudioHelp::getInstance()->playEffect("fridge_on.mp3");
    m_bIsOpen = true;
    m_pBodyDoor->setRotation3D(Vec3(0, 0, 0));
    m_pBodyDoor->runAction(Sequence::create(RotateBy::create(0.5, Vec3(-80, 0, 0)),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pBodyDoor->setVisible(false);
                                                                 m_pBodyDoorOpen->setVisible(true);
                                                                 m_pBodyDoorOpen->setRotation3D(Vec3(80, 0, 0));
                                                                 m_pBodyDoorOpen->runAction(RotateBy::create(0.5, Vec3(-80, 0, 0)));
                                                             }),
                                            DelayTime::create(0.6),
                                            CallFunc::create([=](){
        
//        m_bTouchEnable = true;
        if (onDoorCallback) {
            onDoorCallback(this);
        }
    }),NULL));
}

void OvenNode::closeDoor()
{
    m_bTouchEnable = false;
    AudioHelp::getInstance()->playEffect("fridge_off.mp3");
    m_bIsOpen = false;
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(80,0,0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pBodyDoor->setVisible(true);
                                                                     m_pBodyDoorOpen->setVisible(false);
                                                                     m_pBodyDoor->runAction(RotateBy::create(0.5, Vec3(80, 0, 0)));
                                                                 }),
                                                DelayTime::create(0.6),
                                                CallFunc::create([=](){
        
//        m_bTouchEnable = true;
        if (onDoorCallback) {
            onDoorCallback(this);
        }
    }), NULL));
}

void OvenNode::doBake()
{
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                    MoveBy::create(0.1, Vec2(-4, -4)),
                                                    MoveBy::create(0.1, Vec2(2, 2)),
                                                    NULL)));
    m_pBodyDoor->setTexture("content/make/gummy/oven/oven_3.png");
    if (onBakeCallback) {
        onBakeCallback(this,0);
    }
}

void OvenNode::stopBake()
{
    this->stopAllActions();
    m_pBodyDoor->setTexture("content/make/gummy/oven/oven_2.png");
    m_bBaked = true;
    m_bIsBaking = false;
    if (onBakeCallback) {
        onBakeCallback(this,1);
    }
}


void OvenNode::_onButtonCallbak(Ref* ref,Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED) {
        m_pSwitch->setRotation(90);
        this->setSwitchEnable(false);
        this->setTouchEnable(false);
        doBake();
        m_bIsBaking = true;
        m_pSwitch->runAction(Sequence::create(RotateBy::create(5, -90),
                                              CallFunc::create([=](){
            stopBake();
        }), NULL));
    }
}