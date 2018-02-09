//
//  CBPenSprite.h
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#ifndef __ColorBook__CBPenSprite__
#define __ColorBook__CBPenSprite__

#include "cocos2d.h"
#include "../Model/CBAppGlobal.h"

CB_BEGIN_NAMESPACE

#define OFFSET_X 30
#define OFFSET_Y 0

class PenSprite : public cocos2d::Sprite
{
public:
    CREATE_FUNC(PenSprite);
    
    void showWithAnimation(bool animation, float offsetX = OFFSET_X, float offsetY = OFFSET_Y);
    void restoreWithAnimation(bool animation);
    
    bool isShow(){return _isShow;};
protected:
    bool _isShow;
};



CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBPenSprite__) */
