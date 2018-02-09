//
//  EatLayer.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef EatLayer_hpp
#define EatLayer_hpp
#include "EatSprite.h"
#include "BaseStepLayer.h"
#include "CheckEatPercent.h"
#include "TouchNodeComponent.h"
class EatLayer: public BaseStepLayer{
public:
    ~EatLayer();
    CREATE_SECENE(EatLayer);
    bool init();
    virtual void onEnterTransitionDidFinish();
     virtual void onExitTransitionDidStart();

protected:
    void showEnd();
    void touchEnd(ui::Widget* widge);
    Vec2 _eatAgainPos;
protected:
    void checkEnd();
    void saveAbumle();
    void homeClick();
    static const string _eventName;
    Vector<EatSprite*> foods;
    CheckEatPercent *check;
};

#endif /* EatLayer_hpp */
