
#include "JuiceMachine.h"
#include "AnimationHelp.h"
#include "AudioHelp.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CCArmature.h"
#include "cocostudio/Cocostudio.h"
#include "PolygonHelp.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;

JuiceMachine::JuiceMachine()
{
    onSwitchStateCallback = nullptr;
}

JuiceMachine::~JuiceMachine()
{
    
}
bool JuiceMachine::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    m_nFlavor = 0;
    Node* pCocosNode = CSLoader::createNode("MachineNode.csb");
    
    m_pBodyBack = Sprite::create("content/make/machine/machine_1_down.png");
    this->addChild(m_pBodyBack, 0);
    m_pBodyBack->setPosition(pCocosNode->getChildByName("machine_1_down")->getPosition());
    
    m_pBodyFront = Sprite::create("content/make/machine/machine_1_on.png");
    this->addChild(m_pBodyFront, 10);
    m_pBodyFront->setPosition(pCocosNode->getChildByName("machine_1_on")->getPosition());
    
    m_pSwitch = Button::create("content/make/machine/machine_switch_1.png","content/make/machine/machine_switch_1.png","content/make/machine/machine_switch_1.png");
    this->addChild(m_pSwitch, 11);
    m_pSwitch->addTouchEventListener(CC_CALLBACK_2(JuiceMachine::onButtonCallback, this));
    m_pSwitch->setPosition(pCocosNode->getChildByName("machine_switch_1")->getPosition()+Vec2(0, 30));
    
    m_pOutlet = Sprite::create("content/make/machine/machine_switch_2.png");
    this->addChild(m_pOutlet, 10);
    m_pOutlet->setPosition(pCocosNode->getChildByName("machine_switch_2")->getPosition());
    
    m_pClippingNode = ClippingNode::create(Sprite::create("content/make/machine/machine_fruit juice_mask.png"));
    this->addChild(m_pClippingNode, 1);
    m_pClippingNode->setAlphaThreshold(0.5);
    m_pClippingNode->setPosition(pCocosNode->getChildByName("mask")->getPosition());
    
    m_pFlavorTag = Sprite::create("content/make/machine/taste_label/1.png");
    this->addChild(m_pFlavorTag,12);
    m_pFlavorTag->setPosition(pCocosNode->getChildByName("taste_label")->getPosition());
    
    m_pWaterGrid = NodeGrid::create();
    m_pClippingNode->addChild(m_pWaterGrid);
    m_pWaterGrid->setPosition(Vec2::ZERO);
    
    m_pJuice = Sprite::create("content/make/machine/machine_juice/1.png");
    m_pWaterGrid->addChild(m_pJuice,12);
    m_pJuice->setPosition(pCocosNode->getChildByName("juice")->getPosition()-m_pClippingNode->getPosition());
    
    m_bIsOn = false;
    m_pSwitch->setEnabled(false);
    m_PosSwitch = m_pSwitch->getPosition();
    return true;
}
void JuiceMachine::onExit()
{
    Node::onExit();
    AudioHelp::getInstance()->stopLoopEffect();
}

void JuiceMachine::onButtonCallback(Ref*ref,Widget::TouchEventType type)
{
    if (type==Widget::TouchEventType::BEGAN) {
        _turnOn(true);
    }else if (type==Widget::TouchEventType::ENDED || type==Widget::TouchEventType::CANCELED){
        _turnOn(false);
    }
}

void JuiceMachine::setFlavor(int index)
{
    m_pFlavorTag->setTexture("content/make/machine/taste_label/"+std::to_string(index+1)+".png");
    m_pJuice->setTexture("content/make/machine/machine_juice/"+std::to_string(index+1)+".png");
}

void JuiceMachine::_turnOn(bool on)
{
    m_bIsOn= on;
    if (on) {
        AudioHelp::getInstance()->playEffect("open_close_door.mp3");
        AudioHelp::getInstance()->playLoopEffect("pouring_juice.mp3");
        m_pWaterGrid->runAction(RepeatForever::create(Liquid::create(1, Size(10, 10), 1, 13)));
//        m_pJuice->runAction(MoveTo::create(20, Vec2(0, -m_pJuice->getContentSize().height*0.9)));
        m_pJuice->runAction(RepeatForever::create(Sequence::create(MoveBy::create(2, Vec2(0, -5)),
                                                                   DelayTime::create(0.3), NULL)));
        m_pSwitch->stopAllActions();
        m_pSwitch->setPosition(m_PosSwitch);
        m_pSwitch->setScale(1,0.8);
        m_pSwitch->runAction(MoveTo::create(0.3, m_PosSwitch+Vec2(0, -20)));
        m_pSwitch->runAction(ScaleTo::create(0.3, 1,0.8));
    }else{
        AudioHelp::getInstance()->stopLoopEffect();
        m_pWaterGrid->stopAllActions();
        m_pJuice->stopAllActions();
        m_pSwitch->stopAllActions();
        m_pSwitch->setPosition(m_PosSwitch-Vec2(0, 20));
        m_pSwitch->setScale(1);
        m_pSwitch->runAction(MoveTo::create(0.3, m_PosSwitch+Vec2(0, 20)));
        m_pSwitch->runAction(ScaleTo::create(0.3, 1));
    }
    if (onSwitchStateCallback) {
        onSwitchStateCallback(this,on);
    }
}
