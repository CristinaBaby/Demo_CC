//
//  FryFood.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang on 16/1/23.
//
//

#ifndef FryFood_hpp
#define FryFood_hpp

#include "TouchMoveComponent.h"
#include "BaseStepLayer.h"
class FryFood : public BaseStepLayer{
public:
    CREATE_SECENE(FryFood);
protected:
    bool init();
    virtual void onEnterTransitionDidFinish();
    ParticleSystemQuad* bubble;
      virtual void touchEnd(ui::Widget* widget);
    void moveEnd(Node*,Component*, OperateListner* _lis);
    
    void showPlate();
    Vec2 foodCotainerPos;
    Vec2 palatePos;
};


#endif /* FryFood_hpp */
