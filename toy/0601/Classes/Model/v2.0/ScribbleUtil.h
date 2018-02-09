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
        static Color4B getColor(Image *pImage, Vec2 pPosition);
    };
//}

#endif /* defined(__ScribbleDemo_3_2__Util__) */
