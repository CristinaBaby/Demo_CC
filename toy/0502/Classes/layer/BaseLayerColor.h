//
//  BaseLayerColor.h
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#ifndef __MyGame_Project__BaseLayerColor__
#define __MyGame_Project__BaseLayerColor__

#include <iostream>
#include "cocos2d.h"


class BaseLayerColor : public cocos2d::LayerColor {
    
public:
    
    cocos2d::Sprite* generateSpriteWithParams(const std::string& filePath ,  cocos2d::Vec2 position );
    
    cocos2d::Sprite* generateSpriteFromFrameName(const std::string& frameName ,  cocos2d::Vec2 position );
    
    cocos2d::Sprite* generateBg(const std::string& bgPath);
    
    void addBackFunction(const cocos2d::ccMenuCallback &callback);
public:
    
    cocos2d::Color4B bgColor;
    
};
#endif /* defined(__MyGame_Project__BaseLayerColor__) */
