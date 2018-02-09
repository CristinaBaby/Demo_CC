
#ifndef TouchLayer_h
#define TouchLayer_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class TouchLayer:public Layer
{
public:
    TouchLayer();
    ~TouchLayer();
    bool init();
    
    CREATE_FUNC(TouchLayer);
    std::function<void(Touch *pTouch, Event *pEvent)> onTouchBegan;
    std::function<void(Touch *pTouch, Event *pEvent)> onTouchMoved;
    std::function<void(Touch *pTouch, Event *pEvent)> onTouchEnded;
    
    void setTouchLayerEnable(bool enable = true){
        m_bEnable = enable;
    }
    bool TouchBegan(Touch *pTouch, Event *pEvent);
    void TouchMove(Touch *pTouch, Event *pEvent);
    void TouchEnded(Touch *pTouch, Event *pEvent);
private:
    bool m_bEnable;
};
#endif /* TouchLayer_h */
