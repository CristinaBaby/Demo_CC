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

bool MoveHideShade::MoveHideShade::init(){
    if(TouchMoveComponent::init()){
       addListners(ComponentTouchMove,[](Node* _node,Component*, OperateListner* _lis){
           auto _shade = _node->getChildByName("noshade");
           if(_shade !=nullptr){
               _node->setCascadeOpacityEnabled(false);
               _node->setOpacity(0);
           }
        });
        addListners(ComponentTouchCancle, [](Node* _node,Component*, OperateListner* _lis){
            auto _shade = _node->getChildByName("noshade");
            if(_shade !=nullptr){
                _node->setCascadeOpacityEnabled(false);
                _node->runAction(FadeTo::create(0.15, 255));
                //_node->setOpacity(255);
            }
        });
        return true;
    }
    return false;
}
