//
//  TimerChangeComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/18.
//
//

#include "TimerChangeComponent.h"
IMPLEMENT_COCOS2DX_CLASS(TimerChangeComponent);

TimerChangeComponent::TimerChangeComponent():
_delayPerUnit(2.3)
,_count(-1){
}
TimerChangeComponent:: ~TimerChangeComponent(){
   
}
void TimerChangeComponent::start(){
    if(_enabled&& _count<_changeNodes.size()-1){
        playSound();
        if(!isSchedule(schedule_selector(TimerChangeComponent::changeUpdate)))
            schedule(schedule_selector(TimerChangeComponent::changeUpdate), _delayPerUnit);
        else
            _scheduler->resumeTarget(this);
        if(_owner != nullptr)
            Director::getInstance()->getActionManager()->resumeTarget(_owner);
    }
}
void TimerChangeComponent::stop(){
    pauseSound();
    if(isSchedule(schedule_selector(TimerChangeComponent::changeUpdate)))
        _scheduler->pauseTarget(this);
    if(_owner != nullptr)
        Director::getInstance()->getActionManager()->pauseTarget(_owner);
   
}

void  TimerChangeComponent::onAdd(){
    LQComponent::onAdd();
    for(auto _node:_changeNodes){
        _owner->addChild(_node,1);
        _node->setPosition(_owner->getContentSize()*.5);
    }
}


void TimerChangeComponent::changeUpdate(float){
    if(_owner != nullptr)
        _owner->setCascadeOpacityEnabled(false);
    _count++;
    if(_count == 0){
        for(auto _child:_owner->getChildren()){
            if(_child->getName() != "changing")
                _child->runAction(FadeTo::create(0.2, 0));
        }
        _owner->runAction(FadeTo::create(0.2, 0));
        auto _first = _changeNodes.at(0);
        _first->setOpacity(100);
        _first->runAction(FadeTo::create(0.2, 255));
    }else{
        _changeNodes.at(_count-1)->runAction(FadeTo::create(0.2, 0));
        _changeNodes.at(_count)->setOpacity(100);
        _changeNodes.at(_count)->runAction(Sequence::create(FadeTo::create(0.2, 255),CallFunc::create([this]{
            if(_count == _changeNodes.size()-1){
                _owner->stopAllActions();
                unSchedule(schedule_selector(TimerChangeComponent::changeUpdate));
             
                auto _sprite = dynamic_cast<Sprite*>(_owner);
                if(_sprite != nullptr){
                    _sprite->setSpriteFrame(_changeNodes.at(_count)->getSpriteFrame());
                    for(auto _change:_changeNodes)
                        _change->removeFromParent();
                    _changeNodes.clear();
                }
                _owner->setOpacity(255);
                this->stopSound();
                dispatcherEvent(FrameChangeeEnd);
            }
        }),nullptr));
    }
    dispatcherEventWithData(FrameChange, _count);
   
}

void TimerChangeComponent::setFrameFile(const vector<string>&_frameFile){
    _count = -1;
    _changeNodes.clear();
    for(auto _path:_frameFile){
        auto _s = Sprite::create(_path);
         _s->setName("changing");
        _s->setOpacity(0);
        _changeNodes.pushBack(_s);
        if(_owner != nullptr) {
            _owner->addChild(_s,1);
           _s->setPosition(_owner->getContentSize()*.5);
        }
    }
}