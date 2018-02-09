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
#include "MoveHideShade.h"
#include "DragFall.h"
#include "TimerChangeComponent.h"
class BaseMakeLayer: public BaseStepLayer{
    CC_SYNTHESIZE_RETAIN(ActionInterval*, _mixAction, MixAction);
protected:
     BaseMakeLayer();
    virtual ~BaseMakeLayer();
    virtual void onEnterTransitionDidFinish();
    virtual void onEnter();
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
    DragFall* createDraOperate(Node* owner = nullptr);
protected:
    Vec2 spoonCheckPoint;
    float spoonRote;
    vector<string> mixPath;
};


#endif /* BaseMakeLayer_hpp */
