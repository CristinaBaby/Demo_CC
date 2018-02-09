

#ifndef __AlumnAdapter__
#define __AlumnAdapter__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class AlumnAdapter
{
public:
    //保存图片到相册
    static void saveToAlumn(Image* apCCImg,const std::function<void()>& callback);
    static void finished(bool isOK);
    static void saveFavFinished(bool isOK);
    
    static void loadData();
    static void deleteFavImage(int tag);
    static void saveFavData();
    static void saveToFav(Image* apCCImg,const std::function<void()>& callback);
    static void saveToFav(const char*name,Image* apCCImg,const std::function<void()>& callback);
    //    static void saveToFav(const char*name,CCRenderTexture* apRT);
    static const char* getFavImagePathByID(int tag);
    static Image* getFavIconByID(int tag);
    static Image* getFavImageByID(int tag);
    static const char* getFavDataPathByID(int tag);
    static const char* getFavNameByID(int tag);
    static const char* getFavIconNameByID(int tag);
    
    static int getFavCount();
};

#endif /* defined(__AlumnAdapter__) */
