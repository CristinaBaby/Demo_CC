//
//  BaseScene.h
//  OreoMaker
//
//  Created by wangbosheng1 on 14-10-23.
//
//

#ifndef __OreoMaker__BaseScene__
#define __OreoMaker__BaseScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class BaseScene : public Scene{

public:
    BaseScene();
    virtual ~BaseScene();
    
    void onEnter();
    void onExit();
public:
    virtual bool init();
    CREATE_FUNC(BaseScene);
};
#endif /* defined(__OreoMaker__BaseScene__) */
