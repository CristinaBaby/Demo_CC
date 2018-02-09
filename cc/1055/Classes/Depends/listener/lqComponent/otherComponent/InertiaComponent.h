//
//  InertiaComponent.hpp
//  candyfactory
//
//  Created by luotianqiang1 on 16/4/7.
//
//

#ifndef InertiaComponent_hpp
#define InertiaComponent_hpp

#include "LQComponent.h"
class InertiaComponent: public LQComponent{
    DECLARE_COMPONENT_DYNCLASS(InertiaComponent);
    CC_SYNTHESIZE_RETAIN(ActionInterval*, _invokeAction, InvokeAction);
public:
    InertiaComponent();
    ~InertiaComponent();
    virtual void onAdd();
    virtual void onRemove();
    void Invoke(float percent);
    void stopInvoke();
protected:
    void inertia(float);
    void updateS(float);
    void checkSpeed(float);
protected:
    bool isInvoking;
    float time;
    float currentSpeed;
    vector<float> caches;
    static const int cacheCount;
    int currentIndex;
    std::function<void(float)> updateFunc;
};
#endif /* InertiaComponent_hpp */
