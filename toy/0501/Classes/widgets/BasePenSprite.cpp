//
//  BasePenSprite.cpp
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#include "BasePenSprite.h"

USING_NS_CC;

void BasePenSprite::showWithAnimation(bool animation, float offsetX, float offsetY)
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

void BasePenSprite::restoreWithAnimation(bool animation)
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

