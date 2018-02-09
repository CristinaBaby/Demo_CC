//
//  SwitchSprite.h
//  Make
//
//  Created by QAMAC01 on 15-1-27.
//
//

#ifndef __Make__SwitchSprite__
#define __Make__SwitchSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class SwitchSprite: public Sprite
{
public:
    SwitchSprite* createSpriteWithTexture(const char* pzFilename , bool needSwitch);
    bool initWithTexture(const char* pzFilename , bool needSwitch);
private:
    Sprite* switchSp_;
    bool needSwitch_;
};


#endif /* defined(__Make__SwitchSprite__) */
