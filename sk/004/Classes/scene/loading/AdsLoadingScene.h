//
//  AdsLoadingScene.h
//  Make
//
//  Created by liji on 15/7/14.
//
//

#ifndef __Make__AdsLoadingScene__
#define __Make__AdsLoadingScene__

#include <stdio.h>
#include "cocos2d.h"
#include "AdLoadingLayerBase.h"

USING_NS_CC;

class AdsLoadingScene : public AdLoadingLayerBase
{
public:
    CREATE_FUNC(AdsLoadingScene);
    bool init();
    void onExit();
    void onEnter();

    
    static bool s_showAdsBanner;
    static bool s_showAdsBg;
    static bool s_enableBackClick;
};


#endif /* defined(__Make__AdsLoadingScene__) */
