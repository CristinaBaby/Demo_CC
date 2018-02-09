//
//  FallSprite.h
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#ifndef __Make__FallSprite__
#define __Make__FallSprite__

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

enum FallType
{
    kFallTypeItem,
    kFallTypeBomb,
};


class FallSprite : public Sprite
{
    
public:
    static FallSprite* create(const char* pzFileName, int idx);
    bool initWithFileAndIdx(const char* pzFileName, int idx);
    void startFall();
    FallType type_;

private:

private:
    void update(float dt);
    float accSpeed_ = 0;
};

#endif /* defined(__Make__FallSprite__) */
