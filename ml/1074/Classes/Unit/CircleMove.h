
#ifndef CircleMove_h
#define CircleMove_h

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class CircleMove :public ActionInterval
{
    virtual void update(float time);
public:
    static CircleMove* create(float duration, const cocos2d::Vec2& center, float endRaduis,float angle);
    
    virtual CircleMove* clone() const override;
    virtual CircleMove* reverse(void) const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void stop(void) override;
    bool initWithDuration(float duration, const cocos2d::Vec2& center, float endRaduis,float angle);
    
protected:
    
    float m_duration;
    cocos2d::Vec2 m_center;
//    float m_scaleDiff;
//    float m_currScale;
    float m_angle;
    float m_anglePreFrame;
    int m_frameCnts;
    cocos2d::Vec2 m_initPos;
    float m_angleOrig;
    
//    cocos2d::Vec2 m_moveTo;
    float m_beginRadius;
    float m_endRadius;
//    float m_durationPer;
    
    float m_deltRadius;
    Color4F m_debugColor;
};
#endif /* CircleMove_h */
