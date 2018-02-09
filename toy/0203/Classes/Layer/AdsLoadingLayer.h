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
#include "../ad3_2/AdLoadingLayerBase.h"
#include "CBPicturePage.h"
#include "CBPictureScene.h"
#include "CBPackScene.h"
#include "KDKeyboardDispatcher.h"

class AdsLoadingLayer : public AdLoadingLayerBase,public KDKeyboardDispatcherDelegate
{
public:
    CREATE_FUNC(AdsLoadingLayer);
    
    virtual bool init();
    
    void loadToyboxStudio();
    
    virtual void onAdsLoaded(ADS_TYPE adType);
    
    void onExit() override;
    
    void onEnter() override;
    
    virtual bool onKeyReleased() override;
};

#endif /* defined(__SpotDifferences__AdsLoadingLayer__) */
