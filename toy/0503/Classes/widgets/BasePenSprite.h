//
//  BasePenSprite.h
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#ifndef __BedtimeStory__BasePenSprite__
#define __BedtimeStory__BasePenSprite__

#include "cocos2d.h"
#include "MiniGameManager.h"
#include "cocosGUI.h"

#define OFFSET_X 20
#define OFFSET_Y 0

class BasePenSprite : public cocos2d::ui::ImageView
{
public:
    CREATE_FUNC(BasePenSprite);
    
    void showWithAnimation(bool animation, float offsetX = OFFSET_X, float offsetY = OFFSET_Y);
    void restoreWithAnimation(bool animation);
    
    bool isShow(){return _isShow;};
protected:
    bool _isShow;
};

#endif /* defined(__BedtimeStory__BasePenSprite__) */
