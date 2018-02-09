//
//  IceBoxNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/23/16.
//
//

#include "IceBoxNode.h"

IceBoxNode::~IceBoxNode()
{
    _scheduler->unschedule(schedule_selector(IceBoxNode::progressUpdateTimer), this);
}

bool IceBoxNode::init()
{
    if (!MakeBaseNode::init())
        return false;
    
    _iceBox = Sprite::create(ICE_BOX);
    addChild(_iceBox, 1);
    
    _door = Sprite::create(ICE_BOX_DOOR);
    _door->setOpacity(0);
    addChild(_door, 10);

    _doorBad = Sprite::create(ICE_BOX_DOOR);
    _doorBad->setOpacity(0);
    addChild(_doorBad, 11);
    
    _content = Sprite::create();
    addChild(_content, 3);
    
    _contentUp = Sprite::create(ICE_BOX_CONTENT_UP);
    _contentUp->setOpacity(0);
    addChild(_contentUp, 5);
    
    _contentDown = Sprite::create(ICE_BOX_CONTENT_DOWN);
    _contentDown->setOpacity(0);
    addChild(_contentDown, 2);
    
    _mainContent = _iceBox;
    
    _onOffButton = Button::create(ICE_BOX_OFF);
    _onOffButton->setPosition(Vec2(284.0, -181.0));
    _onOffButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
       
        if (type == Widget::TouchEventType::ENDED)
        {
            if (_isAnimation)
                return;
            
            if (_isOn)
            {
                this->offAction();
            }
            else
            {
                this->onAction();
            }
            
            if (onOffCallback)
                onOffCallback(_isOn);
        }
    });
    addChild(_onOffButton, 11);
    
    _progressTimer = ProgressTimer::create(Sprite::create(ICE_BOX_PROGRESS_BAR));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(0.0, 1.0));
    _progressTimer->setPercentage(0);
    
    auto progressBg = Sprite::create(ICE_BOX_PROGRESS_BG);
    progressBg->setPosition(Vec2(getContentSize().width/2 + 10.0, 0.0));
    _progressTimer->setPosition(progressBg->getContentSize()/2);
    progressBg->addChild(_progressTimer);
    addChild(progressBg, 10);
    
    _okSprite = Sprite::create(ICE_BOX_PROGRESS_OK);
    _okSprite->setScale(0.0);
    _okSprite->setPosition(Vec2(progressBg->getContentSize().width/2, progressBg->getContentSize().height/2 + 60.0));
    progressBg->addChild(_okSprite, 2);
    
    _scheduler->schedule(schedule_selector(IceBoxNode::progressUpdateTimer), this, 1.0/60.0, false);
    
    return true;
}

void IceBoxNode::progressUpdateTimer(float time)
{
    if (!_isOn)
        return;
    
    _froozenTime += time;
    if (froozenTimeCallback)
        froozenTimeCallback(_froozenTime);
    
    CCLOG("froozen time: %f", _froozenTime);
    if (_froozenTime >= 6.0)
    {
        _isOn = false;
        _doorBad->runAction(Sequence::create(FadeIn::create(1.6), CallFunc::create([=](){
            
            offAction();
        }), NULL));
    }
}

void IceBoxNode::setContent(const std::string& file, const Vec2& position)
{
    if (!_content)
    {
        _content = Sprite::create();
        addChild(_content, 3);
    }
    ((Sprite*)_content)->setTexture(file);
    _content->setPosition(position);
}

void IceBoxNode::setContent(Node *content, const Vec2& position)
{
    _content->removeFromParent();
    _content = nullptr;
    
    _content = content;
    addChild(_content, 3);
}

void IceBoxNode::onAction()
{
    _onOffButton->loadTextureNormal(ICE_BOX_ON);
    _froozenTime = 0.0;
    _isOn = true;

    resetAction();

    auto progressTo = ProgressTo::create(6.0, 100.0);
    _progressTimer->runAction(progressTo);

    _door->runAction(FadeIn::create(_froozenDuration));
    _contentDown->runAction(FadeIn::create(_froozenDuration));
    _contentUp->runAction(FadeIn::create(_froozenDuration));
}

void IceBoxNode::resetAction()
{
    getActionManager()->removeAllActionsFromTarget(_progressTimer);
    getActionManager()->removeAllActionsFromTarget(_door);
    getActionManager()->removeAllActionsFromTarget(_contentDown);
    getActionManager()->removeAllActionsFromTarget(_contentUp);
}

void IceBoxNode::offAction()
{
    _onOffButton->loadTextureNormal(ICE_BOX_OFF);
    _isOn = false;
    _doorBad->setOpacity(0);
    
    resetAction();
    if (_isFroozenOk)
    {
        _onOffButton->setTouchEnabled(false);
        if (successFroozenCallback)
            successFroozenCallback();
    }
    else
    {
        _contentUp->runAction(FadeOut::create(0.3));
        _contentDown->runAction(FadeOut::create(0.3));
        _door->runAction(FadeOut::create(0.3));
        _okSprite->runAction(ScaleTo::create(0.3, 0.0));
        
        if (failFroozenCallback)
            failFroozenCallback();
        
        _isAnimation = true;
        _progressTimer->runAction(Sequence::create(ProgressTo::create(0.3, 0.0), CallFunc::create([=](){
            
            _isFroozenOk = false;
            _isAnimation = false;
        }), NULL));
    }
}

void IceBoxNode::froozenGood()
{
    if (_isFroozenOk)
        return;
    
    _isFroozenOk = true;
    _okSprite->runAction(ScaleTo::create(0.3, 1.0));
}

void IceBoxNode::froozenBad()
{
    _isFroozenOk = false;
}



