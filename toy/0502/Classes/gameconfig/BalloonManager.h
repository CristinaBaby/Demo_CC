#pragma once

#include "cocos2d.h"
#include "../AppGlobal.h"
#include "../utilities/UIHeader.h"


#define xBalloon              (BalloonManager::getInstance())

class BalloonManager {
    
public:
    static BalloonManager* getInstance();
    void destroyInstance();
    
    void start();
    void stop();
    void update(float dt);
    
private:
    BalloonManager();
    
private:
    Size    _visibleSize;
    float   _acc = 0;
    int     _count = 0;
};
