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
    auto machineFile = StringUtils::format("res/rainbow_slushy/machine/juice_%d.png",index);
    auto machine = Sprite::create(machineFile);
    addChild(machine, 3);
    
    auto water = Sprite::create(StringUtils::format("res/rainbow_slushy/machine/water%d_1.png",index));
    addChild(water, 2);
    water->runAction(RepeatForever::create(ActionHelper::createAnimate(StringUtils::format("res/rainbow_slushy/machine/water%d",index)+"_%d.png", 1, 2)));

    auto btnOffFile = "res/rainbow_slushy/machine/juice_button1.png";
    _onOffButton = Button::create(btnOffFile);
    _onOffButton->setPosition(Vec2(132, 19)-Vec2(252/2, 414/2)+Vec2(-10, 0));
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

        auto btnOnFile = "res/rainbow_slushy/machine/juice_button2.png";

        _onOffButton->loadTextureNormal(btnOnFile);

        _juiceFlow = Sprite::create(StringUtils::format("res/rainbow_slushy/machine/flow/flow%d_0.png",_juiceIndex+1));
        _juiceFlow->setPosition(Vec2(5,-175));
        _juiceFlow->setScale(1, 1);
        _juiceFlow->setAnchorPoint(Vec2(0.5, 1));
        addChild(_juiceFlow, 2);
        
        _isAnimation = true;
        auto animate = ActionHelper::createAnimate("res/rainbow_slushy/machine/flow/flow" + std::to_string(_juiceIndex+1) + "_%d.png", 0, 3);
        _juiceFlow->runAction(Sequence::create(animate, CallFunc::create([=](){
            
            _juiceFlow->getActionManager()->removeAction(animate);
            auto animate = ActionHelper::createAnimate("res/rainbow_slushy/machine/flow/flow" + std::to_string(_juiceIndex+1) + "_%d.png", 0, 3);
            _juiceFlow->runAction(RepeatForever::create(animate));
            _isAnimation = false;
        }), NULL));
        
        if (pourJuiceCallback)
            pourJuiceCallback();
    }
    else
    {
        
        auto btnOnFile = "res/rainbow_slushy/machine/juice_button2.png";
        
        _onOffButton->loadTextureNormal(btnOnFile);
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


