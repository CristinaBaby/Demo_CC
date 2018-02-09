//
//  STUILayer.h
//  SpotDifference
//
//  Created by Steven.Xc.Tian.
//
//

#ifndef __SpotDifference__STUILayer__
#define __SpotDifference__STUILayer__

#include "cocos2d.h"

class STUILayer : public cocos2d::LayerColor
{
public:
    static STUILayer* create();
    
    static STUILayer * createWithColor(const cocos2d::Color4B& color);
    
    
protected:
    bool initData(const cocos2d::Color4B& color);
    
};

#endif /* defined(__SpotDifference__STUILayer__) */
