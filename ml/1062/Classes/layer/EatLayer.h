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
#include "ShadeSprite.h"
#include "TouchNodeComponent.h"
class EatLayer: public BaseStepLayer{
    DECLARE_DYNCLASS(EatLayer);
public:
    ~EatLayer();
    CREATE_FUNC(EatLayer);
    bool init();
    virtual void onEnterTransitionDidFinish();
     virtual void onExitTransitionDidStart();
    void setFoods(const Map<string,Node*>&);
protected:
    void showEnd();
    void touchEnd(ui::Widget* widge);
    Vec2 _eatAgainPos;
protected:
    void checkEnd();
    void saveAbumle();
    void saveMyMenu();
    void homeClick();
    static const string _eventName;
    Vector<EatSprite*> foods;
    Vector<Node*> showNodes;
    CheckEatPercent *check;
    KDS::ShadeSprite* drink;
    TouchNodeComponent * touchLis;
};

#endif /* EatLayer_hpp */
