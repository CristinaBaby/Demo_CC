//
//  SVGPattern.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/10/29.
//
//

#ifndef __ScribbleDemo_3_2__SVG2PNGFilter__
#define __ScribbleDemo_3_2__SVG2PNGFilter__

#include <stdio.h>
#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;
class TextureLinearFilter {
public:
    static TextureLinearFilter *getInstance();
    RenderTexture *linearTexture(Sprite *pTemplate);
};
#endif /* defined(__ScribbleDemo_3_2__SVGPattern__) */
