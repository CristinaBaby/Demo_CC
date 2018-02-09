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
#include "../ad_3.2/AdCross/AdLoadingLayerBase.h"

class AdsLoadingLayer : public AdLoadingLayerBase
{
public:
    CREATE_FUNC(AdsLoadingLayer);
    
    virtual bool init();
    
    void loadToyboxStudio();
    void loadSkyCastleStudio();
    
    Size _visibleSize;
};

#endif /* defined(__SpotDifferences__AdsLoadingLayer__) */
