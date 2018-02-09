//
//  IceAutoMachineNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/27/16.
//
//

#include "IceAutoMachineNode.h"
#include "TipsUIController.h"
#include "ComponentTouchMove.h"
#include "BowlNode.h"
#include "SoundConfig.h"
IceAutoMachineNode::~IceAutoMachineNode()
{
    
}

bool IceAutoMachineNode::init()
{
    if (!MakeBaseNode::init())
        return false;
    
    auto machine1 = Sprite::create(AUTO_MACHINE1);
    auto machine2 = Sprite::create(AUTO_MACHINE2);
    auto machine3 = Sprite::create(AUTO_MACHINE3);
    _machineDoor = Sprite::create(AUTO_MACHINE_DOOR);
    
    _machineDoor->setAnchorPoint(Vec2(0.0, 0.5));
    _machineDoor->setPosition(Vec2(-95.0, 222.0));
  
    machine3->setTag(123);
    
    addChild(machine1, 1);
    addChild(machine2, 2);
    addChild(machine3, 6);
    addChild(_machineDoor, 7);
    
    _mainContent = machine1;
    
    _onOffButton = Button::create(AUTO_ON_BUTTON);
    _onOffButton->setPosition(Vec2(-7.0, 117.0));
    _onOffButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
       
        if (type == Widget::TouchEventType::ENDED)
        {
            if (!_isEnable)
                return;

            TipsUIController::getInstance()->removeClickTip();
            
            if (_isOn)
                return;
            
            _pressBtnIdd = kAudioUtil->playEffect(kSoundAddWater,true);
            
            //TODO: play effect
            _isOn = true;
            _onOffButton->loadTextureNormal("res/snow_cone/3-6/ice_crusher2_off.png");
            
            runAction(Sequence::create(DelayTime::create(0.3), CallFunc::create([=](){
                
                showIceContent();
            }), DelayTime::create(1.1), CallFunc::create([=](){
                
                showIceContent();
            }), DelayTime::create(1.1), CallFunc::create([=](){
                
                showIceContent();
            }), NULL));
        }
    });
    addChild(_onOffButton, 10);
    
    return true;
}

void IceAutoMachineNode::showIceContent()
{
    std::string file;
    if (_iceContentIndex == 1)
        file = AUTO_ICE_CONTENT1;
    else if (_iceContentIndex == 2)
        file = AUTO_ICE_CONTENT2;
    else if (_iceContentIndex == 3)
        file = AUTO_ICE_CONTENT3;
    
    auto particle = ParticleSystemQuad::create("particle/makeice.plist");
    particle->setPosition(Vec2(0.0, -12.0));
    this->addChild(particle, 3);
    
    auto iceContent = Sprite::create(file);
    iceContent->setOpacity(0);
    iceContent->setTag(124 + _iceContentIndex);
    iceContent->runAction(Sequence::create(FadeIn::create(1.0), CallFunc::create([=](){
   
        _isAnimation = false;
        particle->removeFromParent();

        _iceContentIndex++;
        if (_iceContentIndex == 4)
        {
            runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([=](){
                kAudioUtil->stopEffectBySoundId(_pressBtnIdd);
                if (_makeIceCallback)
                    _makeIceCallback();
            }), NULL));
        }
    }), nullptr));
    this->addChild(iceContent, 5);
}

void IceAutoMachineNode::openDoor(const std::function<void()> callback)
{
    TipsUIController::getInstance()->showClickTip(_machineDoor, _machineDoor->getContentSize()/2);
    
    auto touchComp = ComponentTouchMove::create();
    _machineDoor->addComponent(touchComp);
    touchComp->setIsCanMove(false);
    touchComp->touchClickedCall = [=](Touch *touch){
        
        touchComp->setEnabled(false);
        TipsUIController::getInstance()->removeClickTip();
        _machineDoor->runAction(Sequence::create(RotateBy::create(1.0, -90.0), CallFunc::create([=](){
            
            if (callback)
                callback();
            _machineDoor->removeComponent(touchComp);
        }), NULL));
    };
}

void IceAutoMachineNode::closeDoor(const std::function<void()> callback)
{
    TipsUIController::getInstance()->showClickTip(_machineDoor, _machineDoor->getContentSize()/2);
    
    auto touchComp = ComponentTouchMove::create();
    _machineDoor->addComponent(touchComp);
    touchComp->setIsCanMove(false);
    touchComp->touchClickedCall = [=](Touch *touch){
        
        touchComp->setEnabled(false);
        TipsUIController::getInstance()->removeClickTip();
        _machineDoor->runAction(Sequence::create(RotateBy::create(1.0, 90.0), CallFunc::create([=](){
            
            if (callback)
                callback();
            _machineDoor->removeComponent(touchComp);
        }), NULL));
    };
}

void IceAutoMachineNode::pourOneIce(const std::function<void()>& callback)
{
    auto ice = Sprite::create(AUTO_MACHINE_ICE);
    float x = cocos2d::random(-46.0, 27.0);
    float angel = cocos2d::random(0.0, 360.0);
    ice->setPosition(Vec2(x, 78.0));
    ice->setRotation(angel);
    addChild(ice, 2);
    ice->setOpacity(0);
    ice->runAction(Sequence::create(FadeIn::create(0.3), CallFunc::create(callback), NULL));
}

void IceAutoMachineNode::makeIce(const std::function<void()>& callback)
{
    TipsUIController::getInstance()->showClickTip(_onOffButton, _onOffButton->getContentSize()/2);
    _iceContentIndex = 1;
    _makeIceCallback = callback;
    _isEnable = true;
}

void IceAutoMachineNode::removeBottom()
{
    removeChildByTag(123);
    removeChildByTag(125);
    removeChildByTag(126);
    removeChildByTag(127);
}


