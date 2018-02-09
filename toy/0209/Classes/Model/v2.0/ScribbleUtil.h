//
//  Util.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#ifndef __ScribbleDemo_3_2__Util__
#define __ScribbleDemo_3_2__Util__

#include <stdio.h>
#include <cocos2d.h>
using namespace cocos2d;

//namespace Scribble {

    class ScribbleUtil{
    public:
        static ScribbleUtil* getInstance();
        Color4B getColor(Image *pImage, Vec2 pPosition);
        // 判断OpenGL是否支持某个指定的扩展
        // 若支持，返回true。否则返回false。
        inline bool supportGLExtension(const char* name);
        //获取当前时间（毫秒）
        long getCurrentTimeByMicroseconds();
        Image *getImage(Sprite *pSprite);
    private:
        static ScribbleUtil *s_SharedUtil;
    };
//}

#endif /* defined(__ScribbleDemo_3_2__Util__) */
