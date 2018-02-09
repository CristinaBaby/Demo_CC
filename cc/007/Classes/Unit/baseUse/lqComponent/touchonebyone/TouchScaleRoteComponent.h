//
//  TouchScaleRoteComponent.hpp
//  lqtest
//
//  Created by luotianqiang1 on 15/11/5.
//
//

#ifndef TouchScaleRoteComponent_hpp
#define TouchScaleRoteComponent_hpp

enum class ScaleRoteComponentEvent{
    event_scale_rote,
    event_close,
    event_null
};

#include "TouchComponent.h"
/*
可以放大缩小的Component
 */
class TouchScaleRoteComponent : public TouchComponent{
    //// /***操作按钮的图片*/
    CC_SYNTHESIZE(string, _eventButtonPath, EventButtonPath);
    //  /***关闭按钮的图片*/
    CC_SYNTHESIZE(string, _closeButtonPath, CloseButtonPath);
    //  /***背景虚线的图片*/
    CC_SYNTHESIZE(string, _uiBgPath, UiBgPath);
    CC_SYNTHESIZE(float, _minScale, MinScale);
    CC_SYNTHESIZE(float,_maxScale,MaxScale);
    DECLARE_COMPONENT_DYNCLASS(TouchScaleRoteComponent);
public:
    virtual void onAdd();
    TouchScaleRoteComponent();
    void onResume();
    virtual void onRemove();
    
   // /**隐藏操作按钮*/
    void hideUI();
   // /**显示操作按钮*/
    void showUI();
  
protected:
    virtual void createUI();
    void endTouch();
    void hideSchedlu(float);
    
    virtual bool touchBeganLis(Touch*,Event*);
    virtual void touchMoveLis(Touch*,Event*);
    virtual void touchEndLis(Touch*,Event*);
    virtual void touchCanceLis(Touch*,Event*);
protected:
    Sprite *uiBg, *_eventBtn,*_closeBtn;
    ScaleRoteComponentEvent _option;
    float _orignalLength;
};

#endif /* TouchScaleRoteComponent_hpp */
