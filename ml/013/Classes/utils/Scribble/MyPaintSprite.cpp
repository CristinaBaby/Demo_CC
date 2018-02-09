//
//  MyPaintSprite.cpp
//  MyCppGame
//
//  Created by luotianqiang1 on 9/12/14.
//
//

#include "MyPaintSprite.h"
MyPaintSprite *MyPaintSprite::create(int pCanvasWidth, int pCanvasHeight) {
    MyPaintSprite *ret = new MyPaintSprite();
    if(ret && ret->init(Size(pCanvasWidth, pCanvasHeight)))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MyPaintSprite::useTarget(string pTargetFile){
    auto image = new Image();
    image->initWithImageFile(pTargetFile);
    restorePixes = calcute(image);
    return ScribbleNode::useTarget(pTargetFile);
}


//获取绘出来的点和以前的比例
float MyPaintSprite::getPecent(){
    return calcute(getRender()->newImage())/(restorePixes*1.0f);
}

//获取不透明像素点
long MyPaintSprite::calcute(Image *pImage){
    long lCounter = 1;
    for (long i = 0; i < pImage->getHeight(); ++i) {
        for (long j = 0; j < pImage->getWidth(); ++j) {
            unsigned char *lData = pImage->getData();
            long lPixcelIndex = i * pImage->getWidth() + j;
            unsigned char lRed = lData[lPixcelIndex * 4];
            unsigned char lGreen = lData[lPixcelIndex * 4 + 1];
            unsigned char lBlue = lData[lPixcelIndex * 4 + 2];
            unsigned char lAlpha = lData[lPixcelIndex * 4 + 3];
            if (lAlpha > 10) {
                if (lRed > 0 || lGreen > 0 || lBlue > 0) {
                    ++lCounter;
                }
            }
        }
    }
    delete pImage;
    return lCounter;
}