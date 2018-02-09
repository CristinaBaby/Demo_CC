//
//  CBLoadingScene.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBLoadingScene__
#define __ColorBook__CBLoadingScene__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

class LoadingScene: public cocos2d::Layer
{
public:
    //create scene
    CREATE_SCENE_FUNC(LoadingScene);
    
    //create func
    CREATE_FUNC(LoadingScene);
    
    virtual ~LoadingScene();
    
    virtual bool init();
    
    void finishLoad();

protected:
    cocos2d::ProgressTimer *_progress;
};


CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBLoadingScene__) */
