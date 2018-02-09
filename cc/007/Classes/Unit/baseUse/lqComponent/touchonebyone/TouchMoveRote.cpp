//
//  TouchMoveRote.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#include "TouchMoveRote.h"

IMPLEMENT_COCOS2DX_CLASS(TouchMoveRote);
TouchMoveRote::TouchMoveRote():_eventType(TYPENULL)
{
}

TouchMoveRote::~TouchMoveRote(){
    
}

 bool TouchMoveRote::touchBeganLis(Touch* t,Event* e){
      auto dis = _owner->convertToNodeSpace(t->getLocation()).distance(_owner->getContentSize()*.5);
     auto per = dis/Vec2(_owner->getContentSize()*.5).getLength();
     bool flag = false;
     if(per <1.1)
         flag = true;
     if(flag) {
         points.clear();
         _eventType = TYPENULL;
        
         if(per<0.2)
             _eventType = TYPEMOVE;
         
     }
     return flag;
}

void TouchMoveRote::touchMoveLis(Touch* _touch,Event*){
    if(_eventType == TYPENULL) {
        if(points.size()<10){
            points.push_back(_touch->getLocation());
        }
        else {
            float sum = 0;
            for(int i=0;i<points.size()-2;i++){
                auto a1 = points[i+1]-points[i];
                auto a2 = points[i+2]-points[i+1];
                sum+=a2.getAngle(a1);
            }
            if(abs(sum)<0.4)
                _eventType = TYPEMOVE;
            else
                _eventType = TYPEROTE;
        }
    } else if(_eventType == TYPEROTE) {
        auto _ownPos = _owner->convertToWorldSpace(_owner->getContentSize()*.5);
        Vec2 temp1 = _touch->getPreviousLocation() - _ownPos;
        Vec2 temp2 = _touch->getLocation() - _ownPos;
        float theAngle = CC_RADIANS_TO_DEGREES(temp1.getAngle(temp2));
        _owner->setRotation(_owner->getRotation()-theAngle);
    }else {
        Vec2 _newPoint;
        if(points.size()>0) {
            _newPoint = _owner->getPosition() + _touch->getLocation() - points.at(0);
              points.clear();
        }else {
            _newPoint = _owner->getPosition() +_touch->getDelta();
        }
        //_newPoint = _owner->getParent()->convertToNodeSpace(_touch->getLocation());
        switch (_moveType) {
            case MoveComponentType::kTypeInRect:{
                int _index = getTargetIndex(getCheckPointInPoints(_newPoint));
                if(_index<0){
                    _newPoint = _owner->getPosition();
                    if(_owner != nullptr)
                        _owner->setPosition(_newPoint);
                }
                else{
                    if(_owner != nullptr)
                        _owner->setPosition(_newPoint);
                    dispatcherEventWithData(ComponentTouchMove,_touch->getDelta());
                }
                dispatcherEventWithData(ComponentTouchMoveALL,_touch->getDelta());
            }
                break;
            default:
                if(_owner != nullptr){
                    if(_brintTop&&_owner->getZOrder() != 500)
                        _owner->getParent()->reorderChild(_owner, 500);
                    _owner->setPosition(_newPoint);
                }
              
                break;
        }
    }
      dispatcherEventWithData(ComponentTouchMove,_touch->getDelta());
}



