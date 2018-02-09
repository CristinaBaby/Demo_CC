//
//  CBColorPenSprite.cpp
//  ColorBook
//
//  Created by maxiang on 4/28/15.
//
//

#include "CBColorPenSprite.h"
USING_NS_CC;
CB_BEGIN_NAMESPACE

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
    if (!Sprite::init())
    {
        return false;
    }
    
    bool rcode = false;
    do {
        
        _pen = pen;
        setTexture(_pen->penLogo);
        
        rcode = true;
    } while (0);
    
    return rcode;
}


CB_END_NAMESPACE