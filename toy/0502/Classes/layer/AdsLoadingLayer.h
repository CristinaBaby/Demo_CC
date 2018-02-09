//
//  AdsLoadingLayer.h
//  SpotDifferences
//
//  Created by maxiang on 6/12/15.
//
//

#ifndef __SpotDifferences__AdsLoadingLayer__
#define __SpotDifferences__AdsLoadingLayer__

#include "cocos2d.h"

using namespace cocos2d;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
#include "../modules/AdsLoading/AdCross/AdLoadingLayerBase.h"
class AdsLoadingLayer : public AdLoadingLayerBase
#else
class AdsLoadingLayer : public Layer
#endif
{
public:
    CREATE_FUNC(AdsLoadingLayer);
    
    virtual bool init();
};

#endif /* defined(__SpotDifferences__AdsLoadingLayer__) */
