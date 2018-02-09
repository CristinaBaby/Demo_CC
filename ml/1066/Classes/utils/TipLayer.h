//
//  TipLayer.h
//  KCI1011
//
//  Created by liqiang on 15/10/20.
//
//

#ifndef __KCI1011__TipLayer__
#define __KCI1011__TipLayer__

#include <stdio.h>

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class TipLayer:public LayerColor
{

public:
    TipLayer();
    ~TipLayer();

    bool initwithcolor(const Color4B& color = Color4B(0, 0, 0, 0));
    static TipLayer* create(const Color4B& color = Color4B(0, 0, 0, 0));
    
public:
    void Schedule(float f);
    
public:
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
 
public:
    virtual void onEnter();
    virtual void onExit();
    
public:
    float m_fTime;
    
    void setTime(float f)
    {
        m_fTime = f;
    }
    
    void startSchedule();
    
    void stopSchedule();
    
    std::function<void(TipLayer*)> m_fschedule;
    
    std::function<void(TipLayer*)> m_fTouchbegan;

    bool m_bIsReturn;
    
    void setIsReturn(bool b)
    {
        m_bIsReturn = b;
    }
};


#endif /* defined(__KCI1011__TipLayer__) */
