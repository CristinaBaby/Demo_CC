//
//  CoatePowder.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/23.
//
//

#ifndef CoatePowder_hpp
#define CoatePowder_hpp
#include "TouchMoveComponent.h"
#include "BaseStepLayer.h"
class CoatePowder : public BaseStepLayer{
public:
    CREATE_SECENE(CoatePowder);
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void coate(Node*,Component*,OperateListner*);
    int count;
    int moveCount;
};


#endif /* CoatePowder_hpp */
