#ifndef TouchComponent_h
#define TouchComponent_h

#include "LQComponent.h"
USING_NS_CC;

class TouchComponent :public LQComponent
{
	 DECLARE_COMPONENT_DYNCLASS(TouchComponent);
protected:
    bool isSwallowTouches;
public:
	TouchComponent(void);
	virtual ~TouchComponent(void);
    void setSwallowTouches(bool _flag);
protected:
    virtual EventListener* createLis();
    virtual bool isCotain(Touch* pTouch);
     virtual void onAdd();
   // /**判断世界坐标系中的点是否上node上*/
    bool isPointInNode(Node*,Vec2);
    
    
    virtual bool touchBeganLis(Touch*,Event*);
    virtual void touchMoveLis(Touch*,Event*);
    virtual void touchEndLis(Touch*,Event*);
    virtual void touchCanceLis(Touch*,Event*);
  
};
#endif

