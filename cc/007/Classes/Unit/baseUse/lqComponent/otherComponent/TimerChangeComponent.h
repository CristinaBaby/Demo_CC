//
//  TimerChangeComponent.hpp
//  makeburrito
//
//  Created by luotianqiang1 on 15/11/18.
//
//

#ifndef TimerChangeComponent_hpp
#define TimerChangeComponent_hpp

#include "LQComponent.h"

/**
 根据时间不断切换ower的图片
 */
class TimerChangeComponent: public LQComponent {
    DECLARE_COMPONENT_DYNCLASS(TimerChangeComponent);
    CC_SYNTHESIZE(float, _delayPerUnit, DelayPerUnit);
public:
    TimerChangeComponent();
    ~TimerChangeComponent();
    void setFrameFile(const vector<string>&_frameFile);
    void start();
    void stop();
    virtual void onAdd();
protected:
    void changeUpdate(float);
protected:
    Vector<Sprite*> _changeNodes;
    int _count;
public:
};

#endif /* TimerChangeComponent_hpp */
