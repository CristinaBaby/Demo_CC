//
//  BaseGameLayerColor.h
//  MyGame_Project
//
//  Created by liyang1 on 12/1/14.
//
//

#ifndef __MyGame_Project__BaseGameLayerColor__
#define __MyGame_Project__BaseGameLayerColor__

#include <iostream>
#include "cocos2d.h"
#include "BaseLayerColor.h"



class BaseGameLayerColor : public BaseLayerColor{
    
public:
    std::string baseTargetPath = "";
    
    const int baseTargetSize = 100;
    
    cocos2d::Sprite *bgGameSprite;
public:
    //获取花纹纹理（由基本元素拼接完成）
    cocos2d::RenderTexture* getGameTargetSprite(int w_size, int h_size);
    
    // 绘制精灵得到 Image对象
    cocos2d::RenderTexture* convertFromSpriteToImage(cocos2d::Sprite *sprite);
    
    
    bool checkIsInBounding(cocos2d::Image *image , int imageWidth , int imageHight , cocos2d::Vec2 nodePos);
    //添加用户涂抹的底图（若没有涂抹，则使用默认的黑白线框图 ， 坐标为vec2(0,0)）
    cocos2d::Sprite* addUserDrawBg(const std::string& filePath);
    
    
};
#endif /* defined(__MyGame_Project__BaseGameLayerColor__) */
