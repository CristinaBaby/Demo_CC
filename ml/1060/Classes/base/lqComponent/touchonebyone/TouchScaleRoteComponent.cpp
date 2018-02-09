//
//  TouchScaleRoteComponent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#include "TouchScaleRoteComponent.h"
IMPLEMENT_COCOS2DX_CLASS(TouchScaleRoteComponent);
TouchScaleRoteComponent::TouchScaleRoteComponent():
_closeBtn(nullptr)
,_eventBtn(nullptr)
,uiBg(nullptr)
,_eventButtonPath("scaleroteui/dec_btn_zoom.png")
,_closeButtonPath("scaleroteui/dec_btn_close.png")
,_uiBgPath("scaleroteui/controlTip_bg.png")
,_option(ScaleRoteComponentEvent::event_null)
,_orignalLength(1.0f)
,_minScale(0.5)
,_maxScale(1.5){
    isSwallowTouches = true;
}

void TouchScaleRoteComponent::onAdd(){
    TouchComponent::onAdd();
    createUI();
}

void TouchScaleRoteComponent::createUI(){
    if(uiBg == nullptr){
        uiBg = Sprite::create(_uiBgPath);
        if(uiBg == nullptr) {
            uiBg = Sprite::create();
            uiBg->setContentSize(_owner->getContentSize());
        }
        uiBg->setPosition(_owner->getContentSize()*.5);
        _owner->addChild(uiBg);
        auto _size = uiBg->getContentSize();
        _orignalLength = sqrt(_size.width*_size.width*.25+_size.height*_size.height*.25);
        
        _closeBtn = Sprite::create(_closeButtonPath);
        if(_closeBtn != nullptr) {
            _closeBtn->setPosition(Vec2(0,uiBg->getContentSize().height));
            uiBg->addChild(_closeBtn);
        }
        
        _eventBtn = Sprite::create(_eventButtonPath);
        if(_eventBtn != nullptr) {
            _eventBtn->setPosition(Vec2(uiBg->getContentSize().width, 0));
            uiBg->addChild(_eventBtn);
        }
    }
}

void TouchScaleRoteComponent::endTouch(){
    if(_eventBtn!=nullptr)
        _eventBtn->setColor(Color3B::WHITE);
    if(_closeBtn != nullptr)
        _closeBtn->setColor(Color3B::WHITE);
    schedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu), 1);
}

void TouchScaleRoteComponent::hideUI(){
    if(uiBg->isVisible()){
        unSchedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu));
        uiBg->setVisible(false);
    }
}

void TouchScaleRoteComponent::showUI(){
    if(!uiBg->isVisible()){
        uiBg->setVisible(true);
        schedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu), 1);
    }
}

void TouchScaleRoteComponent::hideSchedlu(float){
    uiBg->setVisible(false);
}

bool TouchScaleRoteComponent::touchBeganLis(Touch* _touch,Event*){
    bool flag = false;
    if(uiBg->isVisible()){
        if(isPointInNode(_closeBtn, _touch->getLocation())){
            unSchedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu));
            _closeBtn->setColor(Color3B::GRAY);
            _option = ScaleRoteComponentEvent::event_close;
            flag = true;
        } else if(isPointInNode(_eventBtn, _touch->getLocation())){
            unSchedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu));
            _eventBtn->setColor(Color3B::GRAY);
            _option = ScaleRoteComponentEvent::event_scale_rote;
            flag = true;
        }
    } else {
        _option = ScaleRoteComponentEvent::event_null;
        if(isPointInNode(_owner, _touch->getLocation())){
            unSchedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu));
            uiBg->setVisible(true);
             _scheduler->schedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu) , this, 0 , 0, 1.5, false);
        }
    }
    return flag;
}
void TouchScaleRoteComponent::touchMoveLis(Touch* _touch,Event*){
    if(_option == ScaleRoteComponentEvent::event_scale_rote){
        auto _ownPos = uiBg->convertToWorldSpace(uiBg->getContentSize()*.5);
        auto distance = _touch->getLocation().distance(_ownPos);
  
        auto _willScle = distance/_orignalLength;
        if(_willScle<_minScale)
            _willScle = _minScale;
        if(_willScle>_maxScale)
            _willScle = _maxScale;
        _owner->setScale(_willScle);
        
        Vec2 temp1 = _touch->getPreviousLocation() - _ownPos;
        Vec2 temp2 = _touch->getLocation() - _ownPos;
        float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
        _owner->setRotation(_owner->getRotation()-theAngle);
    }
}
void TouchScaleRoteComponent::touchEndLis(Touch*,Event*){
    endTouch();
    if(_option == ScaleRoteComponentEvent::event_close){
        dispatcherEvent(removeNode);
        _owner->removeFromParent();
    }
}
void TouchScaleRoteComponent::touchCanceLis(Touch*,Event*){
    endTouch();
}

void TouchScaleRoteComponent::onResume(){
    TouchComponent::onResume();
    schedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu),1.5);
    //_scheduler->schedule(schedule_selector(TouchScaleRoteComponent::hideSchedlu) , this, 0 , 0, 1.5, false);
}


 void TouchScaleRoteComponent::onRemove(){
     TouchComponent::onRemove();
     uiBg->removeFromParent();
     uiBg = nullptr;
}
