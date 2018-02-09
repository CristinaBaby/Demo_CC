//
//  IceManualMachineNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/24/16.
//
//

#include "IceManualMachineNode.h"
#include "TipsUIController.h"
#include "ComponentTouchMove.h"
#include "BowlNode.h"

IceManualMachineNode::~IceManualMachineNode()
{
    
}


bool IceManualMachineNode::init()
{
    if (!MakeBaseNode::init())
        return false;
    
    auto machine1 = Sprite::create(MANUAL_MACHINE1);
    auto machine2 = Sprite::create(MANUAL_MACHINE2);
    auto machine3 = Sprite::create(MANUAL_MACHINE3);
    auto machine4 = Sprite::create(MANUAL_MACHINE4);
    _machineDoor = Sprite::create(MANUAL_MACHINE_DOOR);
    
    _machineDoor->setAnchorPoint(Vec2(0.07, 0.17));
    _machineDoor->setPosition(Vec2(-130.0, 194.0));
    _machineBar = Sprite::create(MANUAL_MACHINE_BAR);
    _machineBar->setAnchorPoint(Vec2(0.0, 0.9));
    _machineBar->setPosition(Vec2(126.0, 125.0));
    
    machine3->setTag(123);
    machine4->setTag(124);

    addChild(machine1, 3);
    addChild(machine2, 2);
    addChild(machine3, 1);
    addChild(machine4, 6);
    addChild(_machineDoor, 7);
    addChild(_machineBar, 7);

    _mainContent = machine1;
    
    return true;
}

void IceManualMachineNode::openDoor(const std::function<void()> callback)
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

void IceManualMachineNode::closeDoor(const std::function<void()> callback)
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

void IceManualMachineNode::pourOneIce(const std::function<void()>& callback)
{
    auto ice = Sprite::create(MANUAL_MACHINE_ICE);
    float x = cocos2d::random(-61.0, 59.0);
    float y = cocos2d::random(207.0, 220.0);
    float angel = cocos2d::random(0.0, 360.0);
    ice->setPosition(Vec2(x, y));
    ice->setRotation(angel);
    addChild(ice, 2);
    ice->setOpacity(0);
    ice->runAction(Sequence::create(FadeIn::create(0.3), CallFunc::create(callback), NULL));
}

void IceManualMachineNode::makeIce(const std::function<void()>& callback)
{
    TipsUIController::getInstance()->showMoveTip(_machineBar, Vec2(_machineBar->getContentSize().width/2, _machineBar->getContentSize().height/2 - 90.0), Vec2(_machineBar->getContentSize().width/2, _machineBar->getContentSize().height/2 + 50.0));
    _iceContentIndex = 1;
    auto touchComp = ComponentTouchMove::create();
    touchComp->setIsCanMove(false);
    _machineBar->addComponent(touchComp);

    _makeProgress = 1;
    touchComp->touchBeganCall = [=](Touch *touch)
    {
        if (_isAnimation || _iceContentIndex == 4)
            return;
        
        TipsUIController::getInstance()->removeAllTips();
    };

    touchComp->touchEndedCall = [=](Touch *touch)
    {
        if (_iceContentIndex >= 4)
             return;
        auto offset = touch->getLocation() - touch->getStartLocation();
        if (std::fabs(offset.y) < 30)
            return;

        if(machineBarClicked)
            machineBarClicked();
        //touchComp->setEnabled(false);
        TipsUIController::getInstance()->removeClickTip();
        _isAnimation = true;
        _machineBar->runAction(Sequence::create(ScaleTo::create(0.5, 1.0, -1.0), ScaleTo::create(0.5, 1.0, 1.0), CallFunc::create([=](){
            
            auto particle = ParticleSystemQuad::create("particle/makeice.plist");
            particle->setPosition(Vec2(0.0, 50.0));
            this->addChild(particle, 5);
            
            std::string file;
            if (_iceContentIndex == 1)
                file = MANUAL_ICE_CONTENT1;
            else if (_iceContentIndex == 2)
                file = MANUAL_ICE_CONTENT2;
            else if (_iceContentIndex == 3)
                file = MANUAL_ICE_CONTENT3;
            
            _iceContentIndex++;
            auto iceContent = Sprite::create(file);
            iceContent->setOpacity(0);
            iceContent->setTag(124 + _iceContentIndex);
            iceContent->runAction(Sequence::create(FadeIn::create(1.0), CallFunc::create([=](){
               
                _isAnimation = false;
                particle->removeFromParent();
                
                if (_iceContentIndex == 4)
                {
                    touchComp->setEnabled(false);
                    
                    if (callback)
                        callback();
                }
            }), NULL));
            this->addChild(iceContent, 5);
        }), NULL));
    };

}

void IceManualMachineNode::removeBottom()
{
    removeChildByTag(123);
    removeChildByTag(124);
    removeChildByTag(125);
    removeChildByTag(126);
    removeChildByTag(127);
    removeChildByTag(128);
}



