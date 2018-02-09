//
//  LoadingScene.h
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-27.
//
//

#ifndef __SpotDifference__LoadingScene__
#define __SpotDifference__LoadingScene__

#include "cocos2d.h"
#include "../utilities/UITools.h"

class LoadingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);
    
private:
    void loadStrings();
    
    void loadMapConfig();
    
    void loadLevelsInfo();
    
    void loadComplete();
    
    LoadingBar * _loading;
    
    void update(float delta);
    
    float _acc;
};



#endif /* defined(__SpotDifference__LoadingScene__) */
