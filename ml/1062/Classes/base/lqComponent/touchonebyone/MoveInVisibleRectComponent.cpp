//
//  MoveInVisibleRectComponent.cpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#include "MoveInVisibleRectComponent.h"
IMPLEMENT_COCOS2DX_CLASS(MoveInVisibleRectComponent);

MoveInVisibleRectComponent::MoveInVisibleRectComponent(){
    setMoveType(MoveComponentType::kTypeInRect);
    setTarget(LQRect::create(Director::getInstance()->getOpenGLView()->getVisibleRect()));
    setSwallowTouches(true);
}