
#ifndef CongratulationLayer_h
#define CongratulationLayer_h

#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

class CongratulationLayer :public Layer
{
public:
    CongratulationLayer();
    ~CongratulationLayer();
    CREATE_FUNC(CongratulationLayer);
    bool init();
    
    void onEnter();
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
private:
};
#endif /* CongratulationLayer_h */
