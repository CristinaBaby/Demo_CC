//
//  MoveHideShade.cpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/18.
//
//

#include "MoveHideShade.h"


IMPLEMENT_COCOS2DX_CLASS(MoveHideShade);
MoveHideShade::MoveHideShade(){
    
}

bool MoveHideShade::init(){
    if(TouchMoveComponent::init()){
       addListners(ComponentTouchMove,[](LQComponent* lq,  OperateListner* _lis){
           auto _node = lq->getOwner();
           auto _shade = _node->getChildByName("noshade");
            auto moving = _node->getChildByName("moving");
           if(_shade !=nullptr || moving != nullptr){
               _node->setCascadeOpacityEnabled(false);
               _node->setOpacity(0);
           }
           if(moving != nullptr)
               moving->setOpacity(255);
           
        });
        addListners(ComponentTouchCancle, [](LQComponent* lq, OperateListner* _lis){
              auto _node = lq->getOwner();
            auto _shade = _node->getChildByName("noshade");
            if(_shade !=nullptr){
                _node->setCascadeOpacityEnabled(false);
                _node->runAction(FadeTo::create(0.15, 255));
                //_node->setOpacity(255);
            }
             auto moving = _node->getChildByName("moving");
            if(moving != nullptr){
                moving->setOpacity(0);
                _node->setOpacity(255);
            }
        });
        return true;
    }
    return false;
}
