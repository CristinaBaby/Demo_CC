//
//  HomeScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#ifndef __OreoMaker__HomeScene__
#define __OreoMaker__HomeScene__

#include <iostream>
#include "cocos2d.h"
#include "../layers/HomeLayer.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class HomeScene: public BaseScene{
private:
    HomeScene();
    virtual ~HomeScene();
public:
    virtual bool init();
    CREATE_FUNC(HomeScene);
};

#endif /* defined(__OreoMaker__HomeScene__) */
