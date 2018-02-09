//
//  EatLayer1.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/19.
//
//

#ifndef EatLayer1_hpp
#define EatLayer1_hpp
#include "EatSprite.h"
#include "BaseStepLayer.h"
#include "CheckEatPercent.h"
#include "TouchNodeComponent.h"
class EatLayer1: public BaseStepLayer{
public:
    ~EatLayer1();
    CREATE_SECENE(EatLayer1);
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
    int playIndex;
};

#endif /* EatLayer1_hpp */
