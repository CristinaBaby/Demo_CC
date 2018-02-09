//
//  FallSprite.cpp
//  Make
//
//  Created by QAMAC01 on 15-2-11.
//
//

#include "FallSprite.h"

static const float fallSpeed = 3;

FallSprite* FallSprite::create(const char* pzFileName, int idx)
{
    FallSprite* pRet = new FallSprite();
    if (pRet && pRet -> initWithFileAndIdx(pzFileName, idx))
    {
        pRet -> autorelease();
        return pRet;
    }
    else
        delete pRet;
    return nullptr;
}

bool FallSprite::initWithFileAndIdx(const char* pzFileName, int idx)
{
    if (!Sprite::initWithFile(pzFileName))
    {
        return false;
    }

    //初始化落物类型
    if (idx != 6)
    {
        type_ = kFallTypeItem;
    }
    else
    {
        type_ = kFallTypeBomb;
    }
    
    
    return true;
}

void FallSprite::startFall()
{
    this -> scheduleUpdate();

}



void FallSprite::update(float dt)
{
    
    float s = fallSpeed + accSpeed_;
    accSpeed_ += 0.05;
    
    this -> setPositionY(this-> getPositionY() - s);
}


