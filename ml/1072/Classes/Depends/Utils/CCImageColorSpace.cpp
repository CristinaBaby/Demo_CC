
/*
 refrence from
 http://blog.csdn.net/ldz15838245189/article/details/9365499
 */


#include "CCImageColorSpace.h"

//Swap
static inline void swapRGB(int *a, int *b){
    *a+=*b;
    *b=*a-*b;
    *a-=*b;
}
//rgb must in [0, 255]
static inline void checkRGB(int *Value)
{
    if (*Value < 0) *Value = 0;
    else if (*Value > 255) *Value = 255;
}
//assign rgb
static inline void assignRGB(int *R, int *G, int *B, int intR, int intG, int intB)
{
    *R = intR;
    *G = intG;
    *B = intB;
}
//set the brightness
static void setBright(int *R, int *G, int *B, int A, int bValue)
{
    int intR = *R;
    int intG = *G;
    int intB = *B;
    if (bValue > 0)
    {
        intR = (intR + (255 - intR) * bValue / 255) * A / 255;
        intG = (intG + (255 - intG) * bValue / 255) * A / 255;
        intB = (intB + (255 - intB) * bValue / 255) * A / 255;
    }
    else if (bValue < 0)
    {
        intR = intR + intR * bValue / 255;
        intG = intG + intG * bValue / 255;
        intB = intB + intB * bValue / 255;
    }
    checkRGB(&intR);
    checkRGB(&intG);
    checkRGB(&intB);
    assignRGB(R, G, B, intR, intG, intB);
}
//set the hue and saturation
static void setHueAndSaturation(int *R, int *G, int *B, int hValue, int sValue)
{
    int intR = *R;
    int intG = *G;
    int intB = *B;
    
    if (intR < intG)
        swapRGB(&intR, &intG);
    if (intR < intB)
        swapRGB(&intR, &intB);
    if (intB > intG)
        swapRGB(&intB, &intG);
    
    int delta = intR - intB;
    if (!delta) return;
    
    int entire = intR + intB;
    int H, S, L = entire >> 1;
    if (L < 128)
        S = delta * 255 / entire;
    else
        S = delta * 255 / (510 - entire);
    
    if (hValue)
    {
        if (intR == *R)
            H = (*G - *B) * 60 / delta;
        else if (intR == *G)
            H = (*B - *R) * 60 / delta + 120;
        else
            H = (*R - *G) * 60 / delta + 240;
        H += hValue;
        if (H < 0) H += 360;
        else if (H > 360) H -= 360;
        int index = H / 60;
        int extra = H % 60;
        if (index & 1) extra = 60 - extra;
        extra = (extra * 255 + 30) / 60;
        intG = extra - (extra - 128) * (255 - S) / 255;
        int Lum = L - 128;
        if (Lum > 0)
            intG += (((255 - intG) * Lum + 64) / 128);
        else if (Lum < 0)
            intG += (intG * Lum / 128);
        checkRGB(&intG);
        switch (index)
        {
            case 1:
                swapRGB(&intR, &intG);
                break;
            case 2:
                swapRGB(&intR, &intB);
                swapRGB(&intG, &intB);
                break;
            case 3:
                swapRGB(&intR, &intB);
                break;
            case 4:
                swapRGB(&intR, &intG);
                swapRGB(&intG, &intB);
                break;
            case 5:
                swapRGB(&intG, &intB);
                break;
        }
    }
    else
    {
        intR = *R;
        intG = *G;
        intB = *B;
    }
    if (sValue)
    {
        if (sValue > 0)
        {
            sValue = sValue + S >= 255? S: 255 - sValue;
            sValue = 65025 / sValue - 255;
        }
        intR += ((intR - L) * sValue / 255);
        intG += ((intG - L) * sValue / 255);
        intB += ((intB - L) * sValue / 255);
        checkRGB(&intR);
        checkRGB(&intG);
        checkRGB(&intB);
    }
    assignRGB(R, G, B, intR, intG, intB);
}

Image* CCImageColorSpace::imageWithHSB(Image *pRGBImage, int pHueValue, int pSaturationValue, int pBrightnessValue) {
    
    if (pHueValue == 0 && pSaturationValue == 0 && pBrightnessValue == 0) {
        return pRGBImage;
    }
    
    pSaturationValue = pSaturationValue * 255 / 100;
    pBrightnessValue = pBrightnessValue * 255 / 100;
    
    if (!pRGBImage->getData()) {
        CCLOG("Create PixelData error!");
        return NULL;
    }
    
    const long lLen = pRGBImage->getDataLen();
    CCLOG("lLen %ld",lLen);
    unsigned char *lData = (unsigned char *)malloc(sizeof(unsigned char) * lLen);
    memset(lData, 0, lLen);
    memcpy(lData, pRGBImage->getData(), lLen);
    
    int pixOff = 0;  //the end of every pixcel
    
    for(int y = 0; y < pRGBImage->getHeight(); y++) {
        for (int x = 0; x < pRGBImage->getWidth(); x++) {
            int red = (unsigned char)lData[pixOff];
            int green = (unsigned char)lData[pixOff + 1];
            int blue = (unsigned char)lData[pixOff + 2];
            int alpha = (unsigned char)lData[pixOff + 3];
            
            
            if (alpha>=100) {
                if ((red|green|blue) != 0) {
                    setHueAndSaturation(&red, &green, &blue, pHueValue, pSaturationValue);
                    setBright(&red, &green, &blue, alpha, pBrightnessValue);
                }
                lData[pixOff] = red;
                lData[pixOff + 1] = green;
                lData[pixOff + 2] = blue;
            }
            pixOff += 4;
        }
    }
    glUniform1f(2,3);
    Image *lImage = new Image();
    lImage->initWithRawData(lData, lLen, pRGBImage->getWidth(), pRGBImage->getHeight(),false);
    lImage->autorelease();
    free(lData);
    return lImage;
}

CCImageColorSpace::RGBA CCImageColorSpace::getRGBA(const std::string& path, int pHueValue, int pSaturationValue, int pBrightnessValue){
    cocos2d::Image* pRGBImage = new Image();
    pRGBImage->initWithImageFile(path);
    
    if (!pRGBImage->getData()) {
        CCLOG("Create PixelData error!");
        return {0,0,0,0};
    }
    
    pSaturationValue = pSaturationValue * 255 / 100;
    pBrightnessValue = pBrightnessValue * 255 / 100;
    
    const long lLen = pRGBImage->getDataLen();
    CCLOG("lLen %ld",lLen);
    unsigned char *lData = (unsigned char *)malloc(sizeof(unsigned char) * lLen);
    memset(lData, 0, lLen);
    memcpy(lData, pRGBImage->getData(), lLen);
    
    
    int pixOff = 0;  //the center pixcel
    int height = pRGBImage->getHeight();
    int width = pRGBImage->getWidth();
    for(int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixOff += 4;
            int alpha = (unsigned char)lData[pixOff + 3];
            if (alpha==255) {
                break;
            }
        }
    }
//    int pixOff = 4*width*height/2+width*2;  //the center pixcel
    
    int red = (unsigned char)lData[pixOff];
    int green = (unsigned char)lData[pixOff + 1];
    int blue = (unsigned char)lData[pixOff + 2];
    int alpha = (unsigned char)lData[pixOff + 3];
    
    if (pHueValue == 0 && pSaturationValue == 0 && pBrightnessValue == 0) {
        return {red,green,blue,alpha};
    }
    setHueAndSaturation(&red, &green, &blue, pHueValue, pSaturationValue);
    setBright(&red, &green, &blue, alpha, pBrightnessValue);
    
    pRGBImage->release();
    return {red,green,blue,alpha};
}

CCImageColorSpace::RGBA CCImageColorSpace::getRGBA(int pHueValue, int pSaturationValue, int pBrightnessValue){
    pSaturationValue = pSaturationValue * 255 / 100;
    pBrightnessValue = pBrightnessValue * 255 / 100;
    
    int red = 255;
    int green = 255;
    int blue = 255;
    int alpha = 255;
    
    if (pHueValue == 0 && pSaturationValue == 0 && pBrightnessValue == 0) {
        return {red,green,blue,alpha};
    }
    setHueAndSaturation(&red, &green, &blue, pHueValue, pSaturationValue);
    setBright(&red, &green, &blue, alpha, pBrightnessValue);
    
    return {red,green,blue,alpha};
}

Texture2D* CCImageColorSpace::textureWithHSB(cocos2d::Image *pRGBImage, int pHueValue, int pSaturationValue, int pBrightnessValue)
{
    Texture2D* lTexture = new Texture2D();
    lTexture->initWithImage(imageWithHSB(pRGBImage, pHueValue, pSaturationValue, pBrightnessValue));
    lTexture->autorelease();
    return lTexture;
}

Texture2D* CCImageColorSpace::textureWithHSB(const std::string& path, int pHueValue, int pSaturationValue, int pBrightnessValue)
{
    Texture2D* lTexture = new Texture2D();
    cocos2d::Image* image = new Image();
    image->initWithImageFile(path);
    lTexture->initWithImage(imageWithHSB(image, pHueValue, pSaturationValue, pBrightnessValue));
    lTexture->autorelease();
    image->release();
    return lTexture;
    
}
