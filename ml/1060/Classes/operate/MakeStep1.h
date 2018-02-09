//
//  MakeStep1.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/13.
//
//

#ifndef MakeStep1_hpp
#define MakeStep1_hpp
#include "BaseStepLayer.h"
#include "TimerChangeComponent.h"
#include "MoveInVisibleRectComponent.h"
#include "MoveHideShade.h"
#include "MixToolOperate.h"
class MakeStep1: public  BaseStepLayer{
    DECLARE_DYNCLASS(MakeStep1);
public:
    CREATE_FUNC(MakeStep1);
protected:
    ~MakeStep1();
    bool init();
    virtual void onEnterTransitionDidFinish();
    void showThing(const vector<string>&);
     virtual void touchEnd(ui::Widget* widget);
  
    void fallThingCallback(Node*,Component*, OperateListner*); //fall
    void TimerChangeCallBack(Node*,Component*,OperateListner*);//mix
protected:
    vector<string> _makethingsName,soundPath,inbowlName;
    int addCount;
    TimerChangeComponent *_mixComponent;
    MixToolOperate* moveOperate,*blendOperate;
    Sprite* _mixSprite;
    Vec2 _spoonPos,blendPos;
    cocostudio::Armature* mixArm;
};

#endif /* MakeStep1_hpp */
