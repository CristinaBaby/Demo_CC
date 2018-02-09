
#ifndef __ImageColorSpace__
#define __ImageColorSpace__

#include <iostream>
/*
 refrence from
 http://blog.csdn.net/ldz15838245189/article/details/9365499
 */

#include "cocos2d.h"

USING_NS_CC;

class CCImageColorSpace  {
public:
    struct RGBA{
        int r;
        int g;
        int b;
        int a;
    };
    Image* imageWithHSB(Image *pRGBImage, int pHueValue, int pSaturationValue, int pBrightnessValue);
    CCImageColorSpace::RGBA getRGBA(const std::string& path, int pHueValue, int pSaturationValue, int pBrightnessValue);
    CCImageColorSpace::RGBA getRGBA(int pHueValue, int pSaturationValue, int pBrightnessValue);
    Texture2D* textureWithHSB(Image *pRGBImage, int pHueValue, int pSaturationValue, int pBrightnessValue);
    Texture2D* textureWithHSB(const std::string& path, int pHueValue, int pSaturationValue, int pBrightnessValue);
};
#endif /* defined(__ImageColorSpace__) */
