//
//  HintView.h
//  Make
//
//  Created by liji on 15/6/2.
//
//

#ifndef __Make__HintView__
#define __Make__HintView__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;
class HintViewDelegate
{
public:
    virtual void hintViewShowOver(){};

};

class HintView : public Layer
{
public:
    CREATE_FUNC(HintView);
    bool init();
    void setDelegate(HintViewDelegate* delegate){delegate_ = delegate;};
    //设置显示几秒后能点击
    void allowTouchHint(float pDelayTime);

private:
private:
    HintViewDelegate* delegate_;
    EventListenerTouchOneByOne* lis_;
};
#endif /* defined(__Make__HintView__) */
