//
//  AccleMoveComponent.cpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/3.
//
//

#include "AccleMoveComponent.h"

IMPLEMENT_COCOS2DX_CLASS(AccleMoveComponent);

AccleMoveComponent::AccleMoveComponent(){
    _magnification = 200;
    _originalPos = Vec2::ZERO;
}

void  AccleMoveComponent::dispatcherAccelEvent(Acceleration * _acceleration, Event *){
    if(_enabled && _owner != nullptr) {
        if(_originalPos == Vec2::ZERO)
            _originalPos = _owner->getPosition();
        auto movePos = Vec2(_acceleration->x*_magnification,0);
        _owner->setPosition(_originalPos+movePos);
        
    }
}