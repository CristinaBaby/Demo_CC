//
//  TouchNodeComponent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/9.
//
//

#include "TouchNodeComponent.h"
IMPLEMENT_COCOS2DX_CLASS(TouchNodeComponent);
TouchNodeComponent::TouchNodeComponent(){
    
}

bool TouchNodeComponent::touchBeganLis(Touch* _touch,Event*){
    if(isPointInNode(_owner, _touch->getLocation())){
            dispatcherEvent(ComponentTouchBegan);
        return true;
    }
    return false;
}
void TouchNodeComponent::touchMoveLis(Touch*_touch,Event*){
    if(isPointInNode(_owner, _touch->getLocation())){
        dispatcherEventWithData(ComponentTouchMove, _touch->getLocation());
    }
}
void TouchNodeComponent::touchEndLis(Touch*_touch,Event*){
    if(isPointInNode(_owner, _touch->getLocation()))
            dispatcherEventWithData(ComponentTouchEnd,_touch->getLocation());
    else
         dispatcherEvent(ComponentTouchCancle);
}
void TouchNodeComponent::touchCanceLis(Touch*_touch,Event*){
    dispatcherEvent(ComponentTouchCancle);
}