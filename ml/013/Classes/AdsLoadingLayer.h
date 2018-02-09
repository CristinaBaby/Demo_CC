//
//  AdsLoadingLayer.h
//  Make
//
//  Created by liji on 15/7/14.
//
//

#ifndef __Make__AdsLoadingLayer__
#define __Make__AdsLoadingLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "AdLoadingLayerBase.h"
#include "cocostudio/CocoStudio.h"
#include "./extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AdsLoadingLayer : public AdLoadingLayerBase
{
public:
    CREATE_FUNC(AdsLoadingLayer);
    bool init();
    void onExit();
    void onEnter();

};


#endif /* defined(__Make__AdsLoadingLayer__) */
