
#include "FreezerNode.h"
#include "AudioHelp.h"
#include "AnimationHelp.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

using namespace cocostudio;

FreezerNode::FreezerNode()
{
    onDoorCallback = nullptr;
    m_bFreezed = false;
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
    
    Node* pFreezerNode = CSLoader::createNode("res/FreezerNode.csb");
    ui::Helper::doLayout(pFreezerNode);
    
    Node* pBodyDoor = pFreezerNode->getChildByName("refrigerator_closed");
    Node* BodyDoorOpen = pFreezerNode->getChildByName("refrigerator_open");
    
    m_pBody = Sprite::create("content/make/gummy/freezer/refrigerator.png");
    this->addChild(m_pBody);
    
    
    m_pBodyDoor = Sprite::create("content/make/gummy/freezer/refrigerator_closed_0.png");
    this->addChild(m_pBodyDoor,1);
    m_pBodyDoor->setAnchorPoint(pBodyDoor->getAnchorPoint());
    m_pBodyDoor->setPosition(pBodyDoor->getPosition());
    
    m_pBodyDoorOpen = Sprite::create("content/make/gummy/freezer/refrigerator_open.png");
    this->addChild(m_pBodyDoorOpen,1);
    m_pBodyDoorOpen->setAnchorPoint(BodyDoorOpen->getAnchorPoint());
    m_pBodyDoorOpen->setPosition(BodyDoorOpen->getPosition());
    
    m_bIsOpen = false;
    m_bTouchEnable = true;
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
        this->runAction(Sequence::create(DelayTime::create(0.5),
                                         CallFunc::create([=]() {
            if (onDoorCallback) {
                onDoorCallback(this);
            }
        }), NULL));
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}


void FreezerNode::openDoor()
{
    AudioHelp::getInstance()->playEffect("fridge_on.mp3");
    m_bIsOpen = true;
    m_bTouchEnable = false;
    m_pBodyDoor->setRotation3D(Vec3(0, 0, 0));
    m_pBodyDoor->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0, -80, 0)),
                                            CallFunc::create([=]()
                                                             {
                                                                 m_pBodyDoor->setVisible(false);
                                                                 m_pBodyDoorOpen->setVisible(true);
                                                                 m_pBodyDoorOpen->setRotation3D(Vec3(0, 80, 0));
                                                                 m_pBodyDoorOpen->runAction(RotateBy::create(0.5, Vec3(0, -80, 0)));
                                                                 if (onDoorCallback) {
                                                                     onDoorCallback(this);
                                                                 }
                                                             }), NULL));
}

void FreezerNode::closeDoor()
{
    AudioHelp::getInstance()->playEffect("fridge_off.mp3");
    m_bIsOpen = false;
    m_bTouchEnable = false;
    m_pBodyDoorOpen->runAction(Sequence::create(RotateBy::create(0.5, Vec3(0,80,0)),
                                                CallFunc::create([=]()
                                                                 {
                                                                     m_pBodyDoor->setVisible(true);
                                                                     m_pBodyDoorOpen->setVisible(false);
                                                                     m_pBodyDoor->runAction(RotateBy::create(0.5, Vec3(0, 80, 0)));
                                                                     if (onDoorCallback) {
                                                                         onDoorCallback(this);
                                                                     }
                                                                 }), NULL));
}

void FreezerNode::doFreeze()
{
    m_bFreezed = false;
    this->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(2, 2)),
                                                    MoveBy::create(0.1, Vec2(-4, -4)),
                                                    MoveBy::create(0.1, Vec2(2, 2)),
                                                    NULL)));
    m_pBodyDoor->runAction(AnimationHelp::createAnimate("content/make/gummy/freezer/closed_", 0, 1,false,true,2));
}
void FreezerNode::stopFreeze()
{
    m_bFreezed = true;
    this->stopAllActions();
}