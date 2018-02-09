//
//  EatScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-11-10.
//
//

#ifndef __OreoMaker__EatScene__
#define __OreoMaker__EatScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
USING_NS_CC;
class EatScene: public BaseScene{
private:
    EatScene();
    virtual ~EatScene();
public:
    virtual bool init();
    CREATE_FUNC(EatScene);
};

#endif /* defined(__OreoMaker__EatScene__) */
