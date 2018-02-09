//
//  JuiceMachine.cpp
//  BHUG1066
//
//  Created by maxiang on 6/28/16.
//
//

#include "JuiceMachine.h"
#include "ActionHelper.h"
#include "SoundConfig.h"
JuiceMachine::~JuiceMachine()
{
    
}

JuiceMachine* JuiceMachine::create(int index)
{
    JuiceMachine *pRet = new JuiceMachine();
    if (pRet && pRet->init(index))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool JuiceMachine::init(int index)
{
    if (!MakeBaseNode::init())
        return false;
    
    _juiceIndex = index;
    auto machineFile = StringUtils::format("res/soda/machine_soda/soda_box%d.png",index);
    auto machine = Sprite::create(machineFile);
    addChild(machine, 3);
    
    _onOffButton = Button::create("res/soda/machine_soda/btn1.png");
    _onOffButton->setPosition(Vec2(132, 19)-Vec2(252/2, 414/2));
    addChild(_onOffButton, 1);
    _onOffButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
       
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!_isSelect || _isAnimation)
                return;
            
            if (_isOn)
                onOffAction(false);
            else
                onOffAction(true);
        }
    });
    _onOffButton->setTouchEnabled(false);
    
    _mainContent = machine;
    return true;
}

void JuiceMachine::setMachineSelect(bool isSelect)
{
    _isSelect = isSelect;
    if (_isSelect)
        _onOffButton->setTouchEnabled(true);
    else
        _onOffButton->setTouchEnabled(false);
}

void JuiceMachine::onOffAction(bool isOn)
{
    _isOn = isOn;
    
    if (_isOn)
    {
        kAudioUtil->playEffect(kSoundAddWater,true);

        _onOffButton->loadTextureNormal("res/soda/machine_soda/btn2.png");
        
        _juiceFlow = Sprite::create(StringUtils::format("res/soda/flow_soda/flow%d_1.png",_juiceIndex));
        _juiceFlow->setPosition(Vec2(131,10)-Vec2(252/2, 414/2));
        _juiceFlow->setScale(1, 1.5);
        _juiceFlow->setAnchorPoint(Vec2(0.5, 1));
        addChild(_juiceFlow, 2);
        
        _isAnimation = true;
        auto animate = ActionHelper::createAnimate("res/soda/flow_soda/flow" + std::to_string(_juiceIndex) + "_%d.png", 1, 3);
        _juiceFlow->runAction(Sequence::create(animate, CallFunc::create([=](){
            
            _juiceFlow->getActionManager()->removeAction(animate);
            auto animate = ActionHelper::createAnimate("res/soda/flow_soda/flow" + std::to_string(_juiceIndex) + "_%d.png", 2, 3);
            _juiceFlow->runAction(RepeatForever::create(animate));
            _isAnimation = false;
        }), NULL));
        
        if (pourJuiceCallback)
            pourJuiceCallback();
    }
    else
    {
        
        _onOffButton->loadTextureNormal("res/soda/machine_soda/btn1.png");
        kAudioUtil->stopAllEffect();

        if (!_juiceFlow)
            return;
        
        _juiceFlow->getActionManager()->removeAllActionsFromTarget(_juiceFlow);
        _juiceFlow->removeFromParent();
        _juiceFlow = nullptr;

//        auto animation = Animation::create();
//        animation->setDelayPerUnit(0.1);
//        animation->addSpriteFrameWithFile("res/soda/flow_soda/flow" + std::to_string(_juiceIndex) + "_3.png");
//        animation->addSpriteFrameWithFile("res/soda/flow_soda/flow" + std::to_string(_juiceIndex) + "_2.png");
//        animation->addSpriteFrameWithFile("res/soda/flow_soda/flow" + std::to_string(_juiceIndex) + "_1.png");
//
//        _isAnimation = true;
//        _juiceFlow->runAction(Sequence::create(Animate::create(animation), CallFunc::create([=](){
//            
//            _juiceFlow->removeFromParent();
//            _juiceFlow = nullptr;
//            _isAnimation = false;
//        }), NULL));
    }
}


