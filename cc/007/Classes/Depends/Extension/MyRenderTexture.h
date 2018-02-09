
#ifndef __MyRenderTexture__
#define __MyRenderTexture__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class MyRenderTexture:public RenderTexture {
    
    
public:
    static MyRenderTexture * create(int w, int h);
    static MyRenderTexture * create(int w ,int h, Texture2D::PixelFormat eFormat, GLuint uDepthStencilFormat);
    float getPercentageTransparent();
    float getAlphaWithLocation(Point point);
};
#endif /* defined(__MyRenderTexture__) */
