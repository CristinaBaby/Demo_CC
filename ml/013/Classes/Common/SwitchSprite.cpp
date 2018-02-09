//
//  SwitchSprite.cpp
//  Make
//
//  Created by QAMAC01 on 15-1-27.
//
//

#include "SwitchSprite.h"


SwitchSprite* SwitchSprite::createSpriteWithTexture(const char* pzFilename , bool needSwitch)
{
    SwitchSprite* pSprite = new SwitchSprite();
    if (pSprite && pSprite -> initWithTexture(pzFilename, needSwitch))
    {
        pSprite -> autorelease();
        return pSprite;
    }
    else
        delete pSprite;
    return nullptr;
    
}

bool SwitchSprite::initWithTexture(const char* pzFilename , bool needSwitch)
{
    const char* fullPathStr = __String::createWithFormat("%s%d.png",pzFilename,needSwitch) -> getCString();
    switchSp_ = Sprite::create(fullPathStr);
    if (switchSp_ && Sprite::init())
    {
        switchSp_ -> setPosition(this -> getPosition());
        this -> addChild(switchSp_);
        return true;
    }
    return false;

}
