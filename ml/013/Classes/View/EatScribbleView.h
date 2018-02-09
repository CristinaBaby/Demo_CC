//
//  EatScribbleView.h
//  Make
//
//  Created by QAMAC01 on 15-4-7.
//
//

#ifndef __Make__EatScribbleView__
#define __Make__EatScribbleView__

#include <stdio.h>
#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class EatScribbleViewDelegate
{
public:
    virtual void scribbleCleared(){};
};

class EatScribbleView : public Layer
{

public:
    CREATE_FUNC(EatScribbleView);
    
    virtual bool init();
    void allowEat();
    void forbidEat();
    void setDelegate(EatScribbleViewDelegate* delegate){delegate_ = delegate;};


private:
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    long calcute(Image *pImage);
    float getPecent();

    
private:
    RenderTexture *_canvas;
    EventListenerTouchOneByOne* listener;
    
    long restorePixels_;
    CC_SYNTHESIZE(Layer*, showLayer_, ShowLayer);
    CC_SYNTHESIZE(bool, checkPercent_ , CheckPercent);
    Layer* eatLayer_;
    EatScribbleViewDelegate* delegate_;
    bool canEat = true;
    
    int tapCount_ = 0;
    bool _checkEatAgain = true;
};

#endif /* defined(__Make__EatScribbleView__) */
