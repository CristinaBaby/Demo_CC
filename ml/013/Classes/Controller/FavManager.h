//
//  FavManager.h
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-15.
//
//

#ifndef __MakeSnowMan__FavManager__
#define __MakeSnowMan__FavManager__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class FavManager
{
public:
    static FavManager* getInstance();
    void addFavIcon(const char *iconName);
    void addFavImage(RenderTexture* rtx);
    void addFavImage();
    __Array* getFavArray();
    void getAllFavImage();

    void init();
    void deleteImageByIdx(int idx);
    
    int getCurrentPhotoTag(){return tag_;};
    void setCurrentPhotoTag(int tag){tag_  = tag;};
    
    
    std::vector<std::string> vec_favStr;
private:
    void checkFirstLaunch();
    __Array* favArray_;
    int tag_;
};
#endif /* defined(__MakeSnowMan__FavManager__) */
