//
//  ShareScene.h
//  OreoMaker
//
//  Created by wusonglin1 on 14-10-24.
//
//

#ifndef __OreoMaker__ShareScene__
#define __OreoMaker__ShareScene__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class ShareScene: public BaseScene{
private:
    ShareScene();
    virtual ~ShareScene();
public:
    virtual bool init();
    CREATE_FUNC(ShareScene);
};
#endif /* defined(__OreoMaker__ShareScene__) */
