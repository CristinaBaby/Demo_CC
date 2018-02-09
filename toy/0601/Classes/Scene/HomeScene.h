//
//  HomeScene.h
//  PaintDress
//
//  Created by maxiang on 6/16/15.
//
//

#ifndef __PaintDress__HomeScene__
#define __PaintDress__HomeScene__

#include "cocos2d.h"
#include "AppConfigs.h"

class HomeScene : public cocos2d::Layer
{
public:
    CREATE_SCENE_FUNC(HomeScene);
    CREATE_FUNC(HomeScene);
    
    virtual bool init();
};

#endif /* defined(__PaintDress__HomeScene__) */
