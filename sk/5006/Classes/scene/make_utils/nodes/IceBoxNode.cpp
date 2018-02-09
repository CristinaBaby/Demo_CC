//
//  IceBoxNode.cpp
//  BHUG1066
//
//  Created by maxiang on 6/23/16.
//
//

#include "IceBoxNode.h"
#include "TipsUIController.h"
#include "SoundConfig.h"
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
    addChild(_door, 20);

    _doorBad = Sprite::create(ICE_BOX_DOOR);
    _doorBad->setOpacity(0);
    addChild(_doorBad, 21);
    
    _content = Sprite::create();
    addChild(_content, 3);
    
    _contentUp = Sprite::create();
    _contentUp->setTexture(ICE_BOX_CONTENT_UP);
    _contentUp->setOpacity(0);
    addChild(_contentUp, 19);
    _contentUp->setPosition(0, -40);
    _contentUp->setScale(0.8f);

    _contentDown = Sprite::create();
    _contentDown->setTexture(ICE_BOX_CONTENT_DOWN);
    _contentDown->setOpacity(0);
    _contentDown->setPosition(0, -40);
    _contentDown->setScale(0.8f);
    addChild(_contentDown, 2);
    
    _mainContent = _iceBox;
    
    _onOffButton = Button::create(ICE_BOX_OFF);
    _onOffButton->setTouchEnabled(false);
    _onOffButton->setPosition(Vec2(224, -121.0));
    _onOffButton->addTouchEventListener([=](Ref*, Widget::TouchEventType type){
       
        if (type == Widget::TouchEventType::ENDED)
        {
            if (_isAnimation)
                return;
            
            if (_isClicked)
            {
                this->runAction(Sequence::create(DelayTime::create(1.2f),
                                                 CallFunc::create([=]
                {
                    _isClicked = false;
                }),
                                                 NULL));
            }
            
            
            _isClicked = true;
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
    addChild(_onOffButton, 100);
    
    _progressTimer = ProgressTimer::create(Sprite::create(ICE_BOX_PROGRESS_BAR));
    _progressTimer->setType(ProgressTimer::Type::BAR);
    _progressTimer->setMidpoint(Vec2(0.0, 0.0));
    _progressTimer->setBarChangeRate(Vec2(1.0, 0.0));
    _progressTimer->setPercentage(0);
    
    _progressBg = Sprite::create(ICE_BOX_PROGRESS_BG);
    _progressBg->setPosition(Vec2(0, 300.0));
    _progressTimer->setPosition(_progressBg->getContentSize()/2);
    _progressBg->addChild(_progressTimer);
    addChild(_progressBg, 10);
    
    _okSprite = Sprite::create();
    _okSprite->setTexture(ICE_BOX_PROGRESS_OK);
    _okSprite->setScale(0.0);
    _okSprite->setPosition(Vec2(_progressBg->getContentSize().width/2, _progressBg->getContentSize().height/2));
    _progressBg->addChild(_okSprite, 2);
    _progressBg->setVisible(false);
    
    _scheduler->schedule(schedule_selector(IceBoxNode::progressUpdateTimer), this, 1.0/60.0, false);
    
    return true;
}

void IceBoxNode::showProgressTimer()
{
    _progressBg->setScale(0.f);
    _progressBg->runAction(EaseBackOut::create(ScaleTo::create(0.4f, 1.f)));
    _progressBg->setVisible(true);
    _onOffButton->setTouchEnabled(true);
}


void IceBoxNode::progressUpdateTimer(float time)
{
    if (!_isOn)
        return;
    
    _froozenTime += time;
    if (froozenTimeCallback)
        froozenTimeCallback(_froozenTime);
    
    CCLOG("froozen time: %f", _froozenTime);
    
    if (_froozenTime>=_frozenDoneZone.x+0.2)
    {
        if (_showOk)
        {
            _showOk = false;
            _okSprite->runAction(ScaleTo::create(0.3, 1.0));
            kAudioUtil->playEffect(kSoundOvenDone);
        }
    }
    
    if (_froozenTime >= _frozenDoneZone.y)
    {
        _isOn = false;
        _doorBad->runAction(Sequence::create(FadeIn::create(0.8f), CallFunc::create([=](){
            
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
    _showOk = true;

    resetAction();

    auto progressTo = ProgressTo::create(_froozenDuration, 100.0);
    _progressTimer->runAction(progressTo);

    _door->runAction(FadeIn::create(_froozenDuration));
    _contentDown->runAction(Sequence::create(DelayTime::create(4.f),
                                             FadeIn::create(_froozenDuration-4.f),
                                             NULL));
    _contentUp->runAction(Sequence::create(DelayTime::create(4.f),
                                             FadeIn::create(_froozenDuration-4.f),
                                             NULL));
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
    
    if(_froozenTime >= _frozenDoneZone.x &&  _froozenTime<=_frozenDoneZone.y)
    {
        froozenGood();
    }

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
//    _okSprite->runAction(ScaleTo::create(0.3, 1.0));
}

void IceBoxNode::froozenBad()
{
    _isFroozenOk = false;
}



