//
//  FavManager.cpp
//  MakeSnowMan
//
//  Created by QAMAC01 on 14-8-15.
//
//

#include "FavManager.h"
#include "GameMaster.h"
#include "KSVisibleRect.h"
//#include "STSystemFunction.h"

USING_NS_CC;


#define kFirstLaunch "firstLaunch"
#define kEverLaunch "everLaunch"

static FavManager *instance = NULL;

FavManager* FavManager::getInstance()
{
    if (instance == NULL)
    {
        instance = new FavManager();
    }
    return instance;
}

void FavManager::init()
{
    favArray_ = __Array::createWithCapacity(20);
    favArray_ -> retain();
}

void FavManager::addFavIcon(const char *iconName)
{
    

}

void FavManager::addFavImage(RenderTexture* tx)
{
    int i = 0;
    checkFirstLaunch();
    if (UserDefault::getInstance() -> getBoolForKey(kFirstLaunch))
    {
        UserDefault::getInstance() -> setIntegerForKey("fileNumber",i);
    }
    else
    {
        i = UserDefault::getInstance() -> getIntegerForKey("fileNumber");
    }
   
    auto str = __String::createWithFormat("favImage_%d.png",  UserDefault::getInstance() -> getIntegerForKey("fileNumber"));
    
    ++i;
    
    UserDefault::getInstance() -> setIntegerForKey("fileNumber",i);
    
    tx->saveToFile(str -> getCString(), Image::Format::PNG);

}

void FavManager::checkFirstLaunch()
{
    if (!UserDefault::getInstance() -> getBoolForKey(kEverLaunch))
    {
        UserDefault::getInstance() -> setBoolForKey(kFirstLaunch, true);
        UserDefault::getInstance() -> setBoolForKey(kEverLaunch, true);
    }
    else
    {
        UserDefault::getInstance() -> setBoolForKey(kFirstLaunch, false);
    }
}

__Array* FavManager::getFavArray()
{
    return favArray_;
}

void FavManager::getAllFavImage()
{
    favArray_ -> removeAllObjects();
    vec_favStr.clear();
    for (int i=0 ; i< UserDefault::getInstance() -> getIntegerForKey("fileNumber"); i++)
    {
        auto str = __String::createWithFormat("favImage_%d.png",i);
        std::string fullPath = FileUtils::getInstance() -> getWritablePath() + str -> getCString();
        if (FileUtils::getInstance() -> isFileExist(fullPath.c_str()))
        {
            favArray_ -> addObject(str);
            vec_favStr.push_back(fullPath);
        }
    }
    
    log("totalFavCount %d " , vec_favStr.size());
}

void FavManager::deleteImageByIdx(int idx)
{
    if(idx < vec_favStr.size())
    {
        remove(vec_favStr.at(idx).c_str());
    }
    
}
