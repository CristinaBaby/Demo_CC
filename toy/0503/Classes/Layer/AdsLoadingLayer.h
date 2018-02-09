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
#include "AdLoadingLayerBase.h"

using namespace cocos2d;

class AdsLoadingLayer : public AdLoadingLayerBase
{
public:
    CREATE_FUNC(AdsLoadingLayer);
    
    virtual bool init();
    
    void loadTbStudio();
};

#endif /* defined(__SpotDifferences__AdsLoadingLayer__) */
