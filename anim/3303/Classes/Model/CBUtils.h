//
//  CBUtils.h
//  ColorBook
//
//  Created by maxiang on 4/21/15.
//
//

#ifndef __ColorBook__CBUtils__
#define __ColorBook__CBUtils__
#include "CBAppGlobal.h"
#include "cocos2d.h"

CB_BEGIN_NAMESPACE

class Utils
{
public:
    static Utils* getInstance();
    static float SCREEN_WIDTH;
    static float SCREEN_HEIGHT;
    
    virtual ~Utils();
    inline cocos2d::Vec2 getScreenCenter() const {return _screenCenter;};
    
protected:
    bool init();
    void setupScreenCoordinate();
protected:
    
    cocos2d::Vec2 _screenCenter;
    
};

CB_END_NAMESPACE

#endif /* defined(__ColorBook__CBUtils__) */
