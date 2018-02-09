//
//  SettingLayer.hpp
//  MexicanFood
//
//  Created by huxixiang on 16/3/29.
//
//

#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "AdapterScreen.h"
#include "ToolSprite.h"
#include "SoundPlayer.h"

USING_NS_CC;

class SettingLayer: public LayerColor, public ToolSpriteDelegate
{
public:
    SettingLayer();
    
    ~SettingLayer();
    
public:
    static SettingLayer* create(const Color4B& color = Color4B(150, 150, 150, 150));
    bool initWithColor(const Color4B& color);

    bool initData();
    bool initUI();
    
public:
    virtual void onTouchUpInBoundingBox(ToolSprite* toolSprite,Touch *pTouch);

};
#endif /* SettingLayer_hpp */
