//
//  AccleRoteComponent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/2.
//
//

#ifndef AccleRoteComponent_hpp
#define AccleRoteComponent_hpp

enum class ACCLE_ROTE_EVENT{
    ACCLE_ROTE_CCW,//逆时针
    ACCLE_ROTE_CW//顺时针
};


/***
 *重力感应倾倒物品的Component
 *
 */
#include "AccleComponent.h"
class AccleRoteComponent: public AccleComponent{
    DECLARE_COMPONENT_DYNCLASS(AccleRoteComponent);
    /**初始旋转角度,不设置默认为开始响应得力感应时候的角度*/
    CC_SYNTHESIZE(float, _originalRote, OriginalRote);
    /**旋转结束角度*/
    CC_SYNTHESIZE(float, _endRote, EndRote);
     /**当rotate与_endRotete的差值大于_backThreshold时触发ACCLE_ROTE_BACK*/
    CC_SYNTHESIZE(float, _backThreshold, BackThreshold);
public:
    AccleRoteComponent();
protected:
    virtual void  dispatcherAccelEvent(Acceleration * _acceleration, Event*);
    virtual void dispatcherRote(float _rote);
protected:
    ACCLE_ROTE_EVENT _rotedDirection;
    string _callbackType;

};

#endif /* AccleRoteComponent_hpp */
