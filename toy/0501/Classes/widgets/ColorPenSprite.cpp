//
//  ColorPenSprite.cpp
//  BedtimeStory
//
//  Created by maxiang on 9/16/15.
//
//

#include "ColorPenSprite.h"

USING_NS_CC;

ColorPenSprite* ColorPenSprite::createWithPenData(Pen *pen)
{
    ColorPenSprite *sprite = new (std::nothrow) ColorPenSprite();
    if (sprite && sprite->initWithPenData(pen))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool ColorPenSprite::initWithPenData(Pen *pen)
{
    if (!ImageView::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _pen = pen;
        loadTexture(_pen->penLogo);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


