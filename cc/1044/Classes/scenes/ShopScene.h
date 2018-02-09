//
//  ShopScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#ifndef __OreoMaker__ShopScene__
#define __OreoMaker__ShopScene__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class ShopScene: public BaseScene{
private:
    ShopScene();
    virtual ~ShopScene();
public:
    virtual bool init();
    CREATE_FUNC(ShopScene);
};
#endif /* defined(__OreoMaker__ShopScene__) */
