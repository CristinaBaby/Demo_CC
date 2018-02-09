//
//  RollStep.hpp
//  MyDonutShop
//
//  Created by luotianqiang1 on 15/12/11.
//
//

#ifndef RollStep_hpp
#define RollStep_hpp
#include "BaseStepLayer.h"
#include "TimerChangeComponent.h"
#include "CutDountComplent.h"
class RollStep: public  BaseStepLayer{
    DECLARE_DYNCLASS(RollStep);
public:
    CREATE_FUNC(RollStep);
protected:
        virtual void touchEnd(ui::Widget* widget);
    bool init();
    virtual void onEnterTransitionDidFinish();
        virtual void onExitTransitionDidStart();
    void toucComponetCallback(Node*,Component*,OperateListner*);//move spoon
    void TimerChangeCallBack(Node*,Component*,OperateListner*);//mix
protected:
    TimerChangeComponent *_changeOperate;
    void showCutter();
    CutDountComplent*pCutOperate;
    Vec2 _starPos;
    static const string _eventName;
};

#endif /* RollStep_hpp */
