

#ifndef __ImageUtil__
#define __ImageUtil__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class ImageUtil {
public:
    //将CCImage转换为UIImage对象，实现了cocos2d-x与ios图片的接轨，可用于分享，照片保存等
    static UIImage* CCImageToUIImage(Image* apImg);
};


#endif /* defined(__ImageUtil__) */
