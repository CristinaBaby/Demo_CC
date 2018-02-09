//
//  PercentChangeComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/18.
//
//

#include "PercentChangeComponent.h"
IMPLEMENT_COCOS2DX_CLASS(PercentChangeComponent);

PercentChangeComponent::PercentChangeComponent():
_count(-1){
    
}

void PercentChangeComponent::invoking(float percent){
    do {
        CC_BREAK_IF(!_enabled);
        initNode();
        _currentPercent = percent;
        if(_currentPercent>1)
            _currentPercent =1;
        else if(_currentPercent<0)
            _currentPercent = 0;
        _count = _currentPercent*_changeNodes.size();
        CC_BREAK_IF(_count>_changeNodes.size());
        playSound();
        
        if(_owner != nullptr)
            _owner->setCascadeOpacityEnabled(false);
        
        
        float frameChange = 1/((float)_changeNodes.size());
        
        auto changePercent = (_currentPercent- _count*frameChange)/frameChange;
        
        if(_count == 0){
            for(auto _child:_owner->getChildren()){
                if(_child->getName() != "changing"){
                    _child->setCascadeOpacityEnabled(true);
                    _child->setOpacity(255*(1-changePercent));
                }
            }
            _owner->setOpacity(255*(1-changePercent));
        }else{
            _changeNodes.at(_count-1)->setOpacity(255*(1-changePercent));
        }
        if(_count<_changeNodes.size())
            _changeNodes.at(_count)->setOpacity(100+155*changePercent);
        //  CCLOG("changePercent==%f",changePercent);
        
        if(abs(1-changePercent)<0.01){
            for(auto _child:_owner->getChildren()){
                if(_child->getName() != "changing"){
                    _child->setOpacity(0);
                }
            }
            for(int i=0;i<_count&&i<_changeNodes.size();i++)
                _changeNodes.at(i)->setOpacity(0);
            _changeNodes.at(_count)->setOpacity(255);
            dispatcherEventWithData(FrameChange, _count);
        }
        if(_currentPercent>=1.0) {
            _owner->stopAllActions();
            auto _sprite = dynamic_cast<Sprite*>(_owner);
            if(_sprite != nullptr){
                _sprite->setSpriteFrame(_changeNodes.at(_count-1)->getSpriteFrame());
                for(auto _change:_changeNodes)
                    _change->removeFromParent();
                _changeNodes.clear();
                _owner->setOpacity(255);
            }
            
            this->stopSound();
            dispatcherEvent(FrameChangeeEnd);
        }
        
    }while(0);
    
}
void PercentChangeComponent::stop(){
    pauseSound();
}

void  PercentChangeComponent::onAdd(){
    LQComponent::onAdd();
    for(auto _node:_changeNodes){
        _owner->addChild(_node,1);
        _node->setPosition(_owner->getContentSize()*.5);
    }
}

float PercentChangeComponent::getCurrentPercent(){
    return _currentPercent;
}


void PercentChangeComponent::initNode(){
    if(_changeNodes.size() == 0){
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
}

void PercentChangeComponent::setFrameFile(const vector<string>&_frameFile){
    this->_frameFile = _frameFile;
    if(isRunning)
        initNode();
}