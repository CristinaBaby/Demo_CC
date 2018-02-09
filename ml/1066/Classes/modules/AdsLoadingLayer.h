//
//  AdsLoadingLayer.h
//  BPLAY4401_HotAirBalloonMaker
//
//  Created by tangbowen on 15/7/3.
//
//

#ifndef __BPLAY4401_HotAirBalloonMaker__AdsLoadingLayer__
#define __BPLAY4401_HotAirBalloonMaker__AdsLoadingLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "AdLoadingLayerBase.h"

USING_NS_CC;

#define     IMAGE_BG_PATH       "images/loading/loading_bg.jpg"
#define     IMAGE_DYNAMIC_PATH  "images/loading/loading_ld.png"

class AdsLoadingLayer : public  AdLoadingLayerBase
{
public:
    AdsLoadingLayer();
    ~AdsLoadingLayer();
    
public:
    static AdsLoadingLayer* create();
    bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    Size _visibleSize;
    
public:
    Vector<Sprite*> m_vecDots;
};

#endif /* defined(__BPLAY4401_HotAirBalloonMaker__AdsLoadingLayer__) */
