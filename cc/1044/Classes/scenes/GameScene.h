//
//  GameScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-22.
//
//

#ifndef __OreoMaker__GameScene__
#define __OreoMaker__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class GameScene: public BaseScene{
private:
    GameScene();
    virtual ~GameScene();
public:
    virtual bool init();
    CREATE_FUNC(GameScene);
};
#endif /* defined(__OreoMaker__GameScene__) */
