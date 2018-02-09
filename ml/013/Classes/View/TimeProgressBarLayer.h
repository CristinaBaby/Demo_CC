//
//  TimeProgressBarLayer.h
//  Make
//
//  Created by QAMAC01 on 15-2-5.
//
//

#ifndef __Make__TimeProgressBarLayer__
#define __Make__TimeProgressBarLayer__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;
class TimeProgressBarLayerDelegate
{
public:
    virtual void timeUp(){};
};

class TimeProgressBarLayer : public Layer
{
public:
    CREATE_FUNC(TimeProgressBarLayer);
    bool init();
    void countingDown();
    void loseTime(float pTime);
    void pause();
    void setDelegate(TimeProgressBarLayerDelegate* delegate){delegate_ = delegate;};
private:
    ProgressTimer* timer_;
    void count(float dt);
    TimeProgressBarLayerDelegate* delegate_;
    void update(float dt);
    float speed_;

};
#endif /* defined(__Make__TimeProgressBarLayer__) */
