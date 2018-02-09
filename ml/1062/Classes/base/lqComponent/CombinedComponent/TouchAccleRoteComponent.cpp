//
//  TouchAccleRoteComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/17.
//
//

#include "TouchAccleRoteComponent.h"
IMPLEMENT_COCOS2DX_CLASS(TouchAccleRoteComponent);

TouchAccleRoteComponent::TouchAccleRoteComponent(){
  
}

 bool TouchAccleRoteComponent::TouchAccleRoteComponent::init(){
     if(AccleRoteComponent::init()){
         auto _touchComponent = TouchNodeComponent::create();
         _touchComponent->addListners(ComponentTouchEnd, [&](Node*,Component*,OperateListner*){
             _owner->setRotation(_endRote);
             this->dispatcherEvent(ACCLE_ROTE_END);
         });
         _touchComponent->setSwallowTouches(false);
         addComponent(_touchComponent);
         return true;
     }
     return false;
}

