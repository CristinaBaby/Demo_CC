//
//  SelectScene.h
//  MLABS1043_BrownieMaker
//
//  Created by wusonglin1 on 15-1-5.
//
//

#ifndef __MLABS1043_BrownieMaker__SelectScene__
#define __MLABS1043_BrownieMaker__SelectScene__

#include <iostream>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"

USING_NS_CC;

class SelectScene: public BaseScene{
private:
    SelectScene();
    virtual ~SelectScene();
public:
    virtual bool init();
    CREATE_FUNC(SelectScene);
};

#endif /* defined(__MLABS1043_BrownieMaker__SelectScene__) */
