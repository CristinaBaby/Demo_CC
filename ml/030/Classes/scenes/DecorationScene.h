//
//  DecorationScene.h
//  MLABS1047_HotDogMaker
//
//  Created by wusonglin1 on 15/8/5.
//
//

#ifndef __MLABS1047_HotDogMaker__DecorationScene__
#define __MLABS1047_HotDogMaker__DecorationScene__

#include <stdio.h>
#include "cocos2d.h"
#include "../Depends/base/BaseScene.h"
USING_NS_CC;
class DecorationScene: public BaseScene{
private:
    DecorationScene();
    virtual ~DecorationScene();
public:
    virtual bool init();
    CREATE_FUNC(DecorationScene);
};
#endif /* defined(__MLABS1047_HotDogMaker__DecorationScene__) */
