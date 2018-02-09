//
//  MoveInVisibleRectComponent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/23.
//
//

#ifndef MoveInVisibleRectComponent_hpp
#define MoveInVisibleRectComponent_hpp

#include "TouchMoveComponent.h"
class MoveInVisibleRectComponent: public TouchMoveComponent{
      DECLARE_COMPONENT_DYNCLASS(MoveInVisibleRectComponent);
public:
    MoveInVisibleRectComponent();
};

#endif /* MoveInVisibleRectComponent_hpp */
