
#ifndef GameUIEvent_h
#define GameUIEvent_h

#include <stdio.h>
#include "SingleTon.h"
#include <functional>
#include <string>
#include "cocos2d.h"

USING_NS_CC;
using namespace cocos2d::ui;

class GameUIEvent :public SingleTon<GameUIEvent>
{
public:
    enum{
        eSceneTagChoosePackage,
        
        eSceneTagSnowconeCrushIce,
        eSceneTagSnowconeAddSlush,
        eSceneTagSnowconeShape,
        eSceneTagSnowconeColor,
        eSceneTagSnowconeDecorate,
        
        eSceneTagCottonChooseFlavor,
        eSceneTagCottonMake,
        eSceneTagCottonChooseShape,
        
        eSceneTagFunnelMix,
        
        eSceneTagDecorate,
        eSceneTagShare,
        eSceneTag,
        
    };
    GameUIEvent();
    ~GameUIEvent();
    int nextSceneTag;
    
    virtual void onButtonCallback(int tag);
};
#endif /* GameUIEvent_h */
