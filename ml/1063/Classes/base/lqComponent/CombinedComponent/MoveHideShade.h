//
//  MoveHideShade.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/18.
//
//

#ifndef MoveHideShade_hpp
#define MoveHideShade_hpp

#include "LQComponent.h"
#include "TouchMoveComponent.h"
class MoveHideShade : public TouchMoveComponent {
    DECLARE_COMPONENT_DYNCLASS(MoveHideShade);
public:
    MoveHideShade();
    virtual bool init();
};

#endif /* MoveHideShade_hpp */
