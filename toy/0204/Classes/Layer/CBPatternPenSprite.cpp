//
//  CBPatternPenSprite.cpp
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#include "CBPatternPenSprite.h"
#include "../Model/CBColouringManager.h"

USING_NS_CC;
CB_BEGIN_NAMESPACE

PatternPenSprite* PatternPenSprite::createWithPatternData(Pattern *pattern)
{
    PatternPenSprite *pen = new (std::nothrow) PatternPenSprite();
    if (pen && pen->initWithPatternData(pattern))
    {
        pen->autorelease();
        return pen;
    }
    CC_SAFE_DELETE(pen);
    return nullptr;
}

bool PatternPenSprite::initWithPatternData(Pattern *pattern)
{
    if (!Sprite::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _pattern = pattern;
        setTexture(_pattern->patternLogo);
        
#if (DEBUG_UNLOCK_PATTERN_PEN)
        _isLocked = false;
#else
        if (pattern->index == 0)//index 0 can be used
        {
            _isLocked = false;
        }
        else
        {
            _isLocked = !ColouringManager::getInstance()->hasUnlockedPatternPen();
            if (_isLocked)
            {
                _lock = Sprite::create("ui04_lock.png");
                CC_BREAK_IF(!_lock);
                _lock->setPosition(Vec2(90, getContentSize().height/2));
                addChild(_lock);
            }
        }
#endif

        rcode = true;
    } while (0);
    
    return rcode;
}

void PatternPenSprite::removeLock()
{
    _isLocked = false;
    if (_lock)
        _lock->removeFromParent();
}

CB_END_NAMESPACE