//
//  FavScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#ifndef __OreoMaker__FavScene__
#define __OreoMaker__FavScene__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class FavScene: public BaseScene{
private:
    FavScene();
    virtual ~FavScene();
public:
    virtual bool init();
    CREATE_FUNC(FavScene);
};
#endif /* defined(__OreoMaker__FavScene__) */
