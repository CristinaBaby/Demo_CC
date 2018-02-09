//
//  CBPenSprite.cpp
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#include "CBPenSprite.h"
USING_NS_CC;
CB_BEGIN_NAMESPACE

void PenSprite::showWithAnimation(bool animation, float offsetX, float offsetY)
{
    if (_isShow)
        return;
    
    if (animation)
    {
        auto action = MoveTo::create(0.1, Vec2(Vec2(getPosition().x - offsetX, getPosition().y - offsetY)));
        runAction(action);
    }
    else
    {
        setPosition(Vec2(getPosition().x - offsetX, getPosition().y - offsetY));
    }
    
    _isShow = true;
}

void PenSprite::restoreWithAnimation(bool animation)
{
    if (!_isShow)
        return;
    
    if (animation)
    {
        auto action = MoveTo::create(0.1, Vec2(Vec2(getPosition().x + OFFSET_X, getPosition().y + OFFSET_Y)));
        runAction(action);
    }
    else
    {
        setPosition(Vec2(getPosition().x + OFFSET_X, getPosition().y + OFFSET_Y));
    }
    
    _isShow = false;
}


CB_END_NAMESPACE