//
//  TryLaterLayer.h
//  ColorBook0818
//
//  Created by huangwen on 15/9/21.
//
//

#ifndef __ColorBook0818__TryLaterLayer__
#define __ColorBook0818__TryLaterLayer__

#include "cocos2d.h"
#include "cocosgui.h"

USING_NS_CC;

class RewardAdLoadingDelegate;

class TryLaterLayer : public Layer {
public:
    static TryLaterLayer* create();
    
    virtual bool init();
    
    void onButton(Ref* pSender, ui::Widget::TouchEventType type);
};

#endif /* defined(__ColorBook0818__TryLaterLayer__) */
