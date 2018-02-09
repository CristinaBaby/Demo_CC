//
//  LockScreenLayer.h
//  KidsMeal
//
//  Created by huxixiang on 1/4/15.
//
//

#ifndef __KidsMeal__LockScreenLayer__
#define __KidsMeal__LockScreenLayer__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class LockScreenLayer: public LayerColor
{
public:
    LockScreenLayer();
    
    ~LockScreenLayer();
    
public:
    static LockScreenLayer* create(const Color4B& color = Color4B(0, 0, 0, 0));
    bool initWithColor(const Color4B& color);

public:
    
    bool initLayer();
};

#endif /* defined(__KidsMeal__LockScreenLayer__) */
