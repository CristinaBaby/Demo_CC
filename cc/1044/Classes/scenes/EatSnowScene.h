//
//  EatSnowScene.h
//  CCATS1044_BeachFoodParty
//
//  Created by wusonglin1 on 15/7/13.
//
//

#ifndef __CCATS1044_BeachFoodParty__EatSnowScene__
#define __CCATS1044_BeachFoodParty__EatSnowScene__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseScene.h"
USING_NS_CC;
class EatSnowScene: public BaseScene{
private:
    EatSnowScene();
    virtual ~EatSnowScene();
public:
    virtual bool init();
    CREATE_FUNC(EatSnowScene);
};
#endif /* defined(__CCATS1044_BeachFoodParty__EatSnowScene__) */
