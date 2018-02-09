//
//  JigsawTileSprite.cpp
//  BedtimeStory
//
//  Created by maxiang on 9/11/15.
//
//

#include "JigsawTileSprite.h"

USING_NS_CC;

JigsawTileSprite::~JigsawTileSprite()
{
    CCLOG("MX: release tile sprite -> %d", getTag());
}

JigsawTileSprite* JigsawTileSprite::createWithTexture(Texture2D *texture)
{
    JigsawTileSprite *sprite = new (std::nothrow) JigsawTileSprite();
    if (sprite && sprite->initWithTexture(texture))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void JigsawTileSprite::removeTileChild(JigsawTileSprite *child)
{
    auto iterator = std::find(_tileChildrenVector.begin(), _tileChildrenVector.end(), child);
    if (iterator != _tileChildrenVector.end()) {
        _tileChildrenVector.erase(iterator);
    }
}


