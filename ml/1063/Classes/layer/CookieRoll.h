//
//  CookieRoll.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/11.
//
//

#ifndef CookieRoll_hpp
#define CookieRoll_hpp
#include "BaseStepLayer.h"
#include "MoveHideShade.h"
class CookieRoll: public  BaseStepLayer{
    DECLARE_DYNCLASS(CookieRoll);
public:
    CREATE_SECENE(CookieRoll);
    CookieRoll();
protected:
    virtual void touchEnd(ui::Widget* widget);
    bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();
    virtual void onExitTransitionDidStart();
    void toucComponetCallback(Node*,Component*,OperateListner*);
    void changeFunc(int index);
    MoveHideShade* _moveOp;
protected:
    static const string _eventName;
};

#endif /* CookieRoll_hpp */
