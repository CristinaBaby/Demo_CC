//
//  STMultiScreenHelper.h
//  SpotDifference
//
//  Created by Steven.Tian on 14-10-27.
//
//

#ifndef __SpotDifference__MultiScreenHelper__
#define __SpotDifference__MultiScreenHelper__

#include "cocos2d.h"
#include "AppConfig.h"

/**
 *
 */
class MultiScreenHelper
{
public:
    /* screen zone */
    static cocos2d::Rect visible_rect;
    
    static inline float getValue(float value)
    {
        return IS_2048 ? value : (value / 2);
    }
    
    static inline cocos2d::Rect getRect(const cocos2d::Vec2& point, const cocos2d::Size& size)
    {
        return cocos2d::Rect(point.x, point.y, size.width, size.height);
    }
    
    static inline cocos2d::Vec2 getPosition(const cocos2d::Vec2& design)
    {
        return getPosition(design.x, design.y);
    }
    
    static inline cocos2d::Vec2 getPosition(float x, float y)
    {
        return IS_2048 ? cocos2d::Vec2(x, y) : cocos2d::Vec2(x / 2, y / 2);
    }
    
    static inline cocos2d::Vec2 getPosition1024(float x, float y)
    {
        return IS_2048 ? cocos2d::Vec2(x*2, y*2) : cocos2d::Vec2(x, y);
    }
    
    static inline cocos2d::Size getSize(float width, float height)
    {
        return IS_2048 ? cocos2d::Size(width, height) : cocos2d::Size(width/2, height/2);
    }
    
    static inline cocos2d::Vec2 screenCenter()
    {
        return cocos2d::Vec2( (getScreenWidth() / 2), (getScreenHeight() / 2) );
    }
    
    static inline cocos2d::Vec2 getCenter(const cocos2d::Vec2& point, const cocos2d::Size& size)
    {
        return cocos2d::Vec2((point.x + size.width/2), (point.y + size.height/2));
    }
    
    static inline cocos2d::Vec2 horizontalCenter(float y)
    {
        return IS_2048 ? cocos2d::Vec2(getScreenWidth() / 2, y) : cocos2d::Vec2(getScreenWidth() / 2, y/2);
    }
    
    static inline cocos2d::Vec2 verticalCenter(float x)
    {
        return IS_2048 ? cocos2d::Vec2(x, (getScreenHeight() / 2)) : cocos2d::Vec2(x/2, (getScreenHeight() / 2));
    }
    
    static inline cocos2d::Vec2 fixedX(float x, float y)
    {
        return IS_2048 ? cocos2d::Vec2(x, y) : cocos2d::Vec2(x, y/2);
    }
    
    static inline cocos2d::Vec2 fixedY(float x, float y)
    {
        return IS_2048 ? cocos2d::Vec2(x, y) : cocos2d::Vec2(x/2, y);
    }
    
    
    static inline float getScreenTop()
    {
        return IS_2048 ?  (1536 * UI_SCALE) : (768 * UI_SCALE);
    }
    
    static inline float getScreenHeight()
    {
        return getScreenTop();// - MultiScreenHelper::visible_rect.origin.y;
    }
    
    static inline float getScreenWidth()
    {
        return IS_2048 ?  2048.f : 1024.f;
    }
    
    static cocos2d::Rect getVisibleRect()
    {
        return visible_rect;
    }
    
    // horizontal
    constexpr static const float DESIGN_WIDTH_HORIZONTAL        = 1024.f;
    constexpr static const float DESIGN_HEIGHT_HORIZONTAL       = 768.f;
};

#endif /* defined(__SpotDifference__MultiScreenHelper__) */
