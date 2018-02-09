//
//  TouchLineComonent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/20.
//
//

#include "TouchLineComonent.h"
#include "LQMath.h"

IMPLEMENT_COCOS2DX_CLASS(TouchLineComonent);
TouchLineComonent::TouchLineComonent():
_direction(Vec2::ZERO)
,_lineCallback(nullptr){
    
}

TouchLineComonent::~TouchLineComonent(){

}

void TouchLineComonent::touchMoveLis(Touch*_touch,Event*){
   // if(isPointInNode(_owner, _touch->getLocation())){
        auto _shade = LQMath::LQgetShadow(_touch->getDelta(), _direction);
        if(_lineCallback != nullptr)
            _lineCallback(_owner,this,_shade);
        dispatcherEvent(ComponentTouchMove);
    //}
}

 void TouchLineComonent::touchEndLis(Touch*_touch,Event*_event){
     TouchNodeComponent::touchEndLis(_touch, _event);
}
 void TouchLineComonent::touchCanceLis(Touch*_touch,Event*_event){
     TouchNodeComponent::touchCanceLis(_touch, _event);
}


