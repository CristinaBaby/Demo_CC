//
//  AdvanceMixerModel.cpp
//  BHUG1062
//
//  Created by tanshoumei on 4/29/16.
//
//

#include "AdvanceBlenderModel.h"
using namespace std;

#define kAdvanceBlenderParticleTag 100

bool AdvanceBlenderModel::init()
{
    setScale(1.1);
    _body = Sprite::create(resRootPath + "blender.png");
    _body->setPosition(12, 0);
    addChild(_body);
    
    _head = Sprite::create(resRootPath + "blender1_0.png");
    addChild(_head);
    
    _particleSprite = Sprite::create(resRootPath + "yellow_flour1.png");
    _body->addChild(_particleSprite);
    _particleSprite->setPosition(Vec2(62, 0));
    Vector<SpriteFrame*> vFrames;
    for (int i = 1; i <= 4; ++i)
    {
        vFrames.pushBack(Sprite::create("donut/make/tool/blender/yellow_flour" + to_string(i) + ".png")->getSpriteFrame());
    }
    auto lAnimation = Animation::createWithSpriteFrames(vFrames, 0.15);
    auto lAnimate = Animate::create(lAnimation);
    auto action = RepeatForever::create(lAnimate);
    action->setTag(_animationTag);
    _particleSprite->setSpriteFrame(vFrames.at(0));
    _particleSprite->runAction(action);
    _particleSprite->setVisible(false);
    
    _status = kAdvanceBlenderStatusIdle;
    return true;
}

Sprite* AdvanceBlenderModel::getBody()
{
    return _body;
}

void AdvanceBlenderModel::setStatus(AdvanceBlenderStatus status)
{
    if(_status == status)
        return;
    _status = status;
    switch (_status) {
        case kAdvanceBlenderStatusIdle:
            stopAnimation();
            break;
        case kAdvanceBlenderStatusTurningIn:
            runTurningAnimationIn();
            break;
        case kAdvanceBlenderStatusTurningOut:
            runTurningAniamtionOut();
            break;
        
        default:
            break;
    }
}

void AdvanceBlenderModel::runTurningAnimationIn()
{
    stopAnimation();
    Vector<SpriteFrame*> vFrames;
    for (int i = 1; i <= 3; ++i)
    {
        vFrames.pushBack(Sprite::create(resRootPath + "blender" + to_string(i) + "_0.png")->getSpriteFrame());
    }
    auto lAnimation = Animation::createWithSpriteFrames(vFrames, 0.15);
    auto lAnimate = Animate::create(lAnimation);
    auto action = RepeatForever::create(lAnimate);
    action->setTag(_animationTag);
    _head->setSpriteFrame(vFrames.at(0));
    _head->runAction(action);
    
    _particleSprite->setVisible(true);
}

void AdvanceBlenderModel::runTurningAniamtionOut()
{
    stopAnimation();
    Vector<SpriteFrame*> vFrames;
    for (int i = 1; i <= 3; ++i)
    {
        vFrames.pushBack(Sprite::create(resRootPath + "blender" + to_string(i) + ".png")->getSpriteFrame());
    }
    auto lAnimation = Animation::createWithSpriteFrames(vFrames, 0.15);
    auto lAnimate = Animate::create(lAnimation);
    auto action = RepeatForever::create(lAnimate);
    action->setTag(_animationTag);
    
    _head->setSpriteFrame(vFrames.at(0));
    _head->runAction(action);
    _particleSprite->setVisible(false);
}

void AdvanceBlenderModel::stopAnimation()
{
    _head->stopAllActions();
    _particleSprite->setVisible(false);
}
