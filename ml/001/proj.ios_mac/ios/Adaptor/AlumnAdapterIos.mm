//
//  AlumnAdapter.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#include "AlumnAdapter.h"
#include "ImageUtil.h"
#import "AlumnImpIos.h"
#include "Defines.h"
#include "ImageUtil.h"
#include "MakeFavManager.h"
#include "NotificationDef.h"

static std::function<void()> saveAlumnCallback = nullptr;
static std::function<void()> saveFavCallback = nullptr;

void AlumnAdapter::saveToAlumn(cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
    saveAlumnCallback = callback;
    [[AlumnImpIos shared] saveToAlumn: ImageUtil::CCImageToUIImage(apCCImg)];
}

void AlumnAdapter::finished(bool isOK)
{
    if (saveAlumnCallback) {
        saveAlumnCallback();
    }
    __NotificationCenter::getInstance()->postNotification(CameraFinishedNotify);
}

void AlumnAdapter::saveFavFinished(bool isOK)
{
    if (saveFavCallback) {
        saveFavCallback();
    }
}

void AlumnAdapter::deleteFavImage(int tag)
{
    [[MakeFavManager sharedFavmanager]deleteFavByID:tag];
}

void AlumnAdapter::saveFavData()
{
    [[MakeFavManager sharedFavmanager]saveFavData];
}
void AlumnAdapter::saveToFav(cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
    saveFavCallback = callback;
    [[MakeFavManager sharedFavmanager] addFavWithName:[[NSDate date]description] Image:ImageUtil::CCImageToUIImage(apCCImg)];
}

void AlumnAdapter::saveToFav(const char *name, cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
    saveFavCallback = callback;
    NSString* str = [NSString stringWithUTF8String:name];
    [[MakeFavManager sharedFavmanager] addFavWithName:str Image:ImageUtil::CCImageToUIImage(apCCImg)];
}

const char* AlumnAdapter::getFavImagePathByID(int tag)
{
    NSString* str = [[MakeFavManager sharedFavmanager]getFavImagePathByID:tag];
    return [str UTF8String];
    
}
Image* AlumnAdapter::getFavIconByID(int tag)
{
    NSString* path =[[MakeFavManager sharedFavmanager]getFavIconPathByID:tag];
    Image* img = new Image();
    img->initWithImageFile([path UTF8String]);
    img->autorelease();
    return img;
}

Image* AlumnAdapter::getFavImageByID(int tag)
{
    
    NSString* path =[[MakeFavManager sharedFavmanager]getFavImagePathByID:tag];
    Image* img = new Image();
    img->initWithImageFile([path UTF8String]);
    img->autorelease();
    return img;
}

const char* AlumnAdapter::getFavIconNameByID(int tag)
{
    NSString* str = [[MakeFavManager sharedFavmanager]getFavIconPathByID:tag];
    return [str UTF8String];
    
}


int AlumnAdapter::getFavCount()
{
    return [[MakeFavManager sharedFavmanager]getFavCount];
}