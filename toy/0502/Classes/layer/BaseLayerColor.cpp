//
//  BaseLayerColor.cpp
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#include "BaseLayerColor.h"
#include "../AppGlobal.h"
#include "../utilities/STVisibleRect.h"
USING_NS_CC;



Sprite* BaseLayerColor::generateSpriteWithParams(const std::string &filePath, cocos2d::Vec2 position )
{
    Sprite*  sprite = Sprite::create(filePath);
    CCASSERT(sprite, "新建精灵失败");
    V::setPosition(sprite, position);
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    return sprite;
}



Sprite* BaseLayerColor::generateBg(const std::string &bgPath)
{
    Size vSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
    Sprite*  sprite = Sprite::create(bgPath);
    CCASSERT(sprite, "新建背景失败");
    V::setPosition(sprite, ccp_center_screen);
//    sprite->setPosition(ccp_center_screen);
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
//    sprite->setAnchorPoint(Vec2(0, 0));
    sprite->setScale(vSize.width/1024);
    return sprite;
}


Sprite* BaseLayerColor::generateSpriteFromFrameName(const std::string &frameName, cocos2d::Vec2 position)
{
    Sprite*  sprite = Sprite::createWithSpriteFrameName(frameName);
    CCASSERT(sprite, "从frame中新建精灵失败");
//    sprite->setPosition(position);
    V::setPosition(sprite, position);
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    return sprite;
}


void BaseLayerColor::addBackFunction(const ccMenuCallback &callback)
{
    Sprite*  backSprite = Sprite::createWithSpriteFrameName("ui04_btn_back.png");
    auto musicItem = MenuItemSprite::create(backSprite, backSprite , callback);
    Menu *menu = Menu::create(musicItem, NULL);
    menu->setPosition(Vec2(backSprite->getContentSize().width*0.8, VISIBLE_RECT.getMaxY() - backSprite->getContentSize().height*0.8));
    addChild(menu);
}