//
//  BaseMakeLayer.hpp
//  DeepFryAnythin
//
//  Created by luotianqiang1 on 16/1/21.
//
//

#ifndef BaseMakeLayer_hpp
#define BaseMakeLayer_hpp

#include "BaseStepLayer.h"
#include "DragEndPosFall.h"
#include "MoveHideShade.h"
#include "TimerChangeComponent.h"
class BaseMakeLayer: public BaseStepLayer{
protected:
     BaseMakeLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void inintNecessary(const string&,const vector<string>&);
    void showNode(const string&,ActionHelper::ShowDirection,float delayTime = 0);
    void touchEnd(ui::Widget* widget);
   virtual void mixEnd(Node*node,Component*, OperateListner* _lis);
   virtual void fallEnd(Node*node,Component*, OperateListner* _lis);
    void showSpoon();
    int count,addEndNum;
protected:
    MoveHideShade* moveSpoon;
    TimerChangeComponent *mix;
    void scaleBowl();
    void scaleBackBowl();
    DragEndPosFall* createDraOperate();
};


#endif /* BaseMakeLayer_hpp */
