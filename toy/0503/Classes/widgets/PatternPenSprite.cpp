//
//  PatternPenSprite.cpp
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#include "PatternPenSprite.h"

USING_NS_CC;

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
    if (!ImageView::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _pattern = pattern;
        loadTexture(_pattern->patternLogo);

        rcode = true;
    } while (0);
    
    return rcode;
}

