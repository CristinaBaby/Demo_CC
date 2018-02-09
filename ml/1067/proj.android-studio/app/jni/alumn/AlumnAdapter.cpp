//
//  AlumnAdapter.cpp
//  Doctor2
//
//  Created by tanshoumei on 13-9-18.
//
//

#include "AlumnAdapter.h"
#include "STSystemFunction.h"
#include "FileUtility.h"
#include "CocosHelper.h"
#include "heads.h"
#include "Dialog.h"
//#include "FavManager.h"

static std::function<void()> saveAlumnCallback = nullptr;
static std::function<void()> saveFavCallback = nullptr;

using namespace std;
void AlumnAdapter::saveToAlumn(cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
//    saveAlumnCallback = callback;
//    STSystemFunction sf;
//    
//    __String* _time=__String::createWithFormat("%ld", CocosHelper::milliSecondNow());
//    FileUtility::createDirectory((FileUtility::getStoragePath()+"/Fair Food").c_str());
//    string path=sf.getSdCardPath()+"/Fair Food/"+_time->_string+".jpg";
//    
//    CCLOG("Android: AlumnAdapter::saveToAlumn at %s", path.c_str());
//    bool issuccess=apCCImg->saveToFile(path.c_str());
//    if (issuccess)
//    {
//		Dialog* dialog=Dialog::create(CCSize2(600,330), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
//		dialog->setContentText("Your image was saved to your Camera Roll!");
//		CCDirector::sharedDirector()->getRunningScene()->addChild(dialog, 9999, 9999);
//        
//        dialog->setPosition(CCDirector::sharedDirector()->getVisibleOrigin());
//    }else
//    {
////        sf.popAlertDialog("Your image was unable to be saved. Please check your device storage and try again.");
//		Dialog* dialog=Dialog::create(CCSize2(600,330), (void*)&MENU_TYPE_1, Dialog::DIALOG_TYPE_SINGLE);
//		dialog->setContentText("Your image was unable to be saved. Please check your device storage and try again.");
//		CCDirector::sharedDirector()->getRunningScene()->addChild(dialog, 9999, 9999);
//        dialog->setPosition(CCDirector::sharedDirector()->getVisibleOrigin());
//    }
//    sf.refreshDCIM(path);
//    
//    AlumnAdapter::finished(true);
}

void AlumnAdapter::finished(bool isOK)
{
//    if (saveAlumnCallback) {
//        saveAlumnCallback();
//    }
//    __NotificationCenter::getInstance()->postNotification(CameraFinishedNotify);
}

void AlumnAdapter::saveFavFinished(bool isOK)
{
//    if (saveFavCallback) {
//        saveFavCallback();
//    }
}


void AlumnAdapter::deleteFavImage(int tag)
{
//    FavManager::getInstance()->deleteFavImage(tag);
}

void AlumnAdapter::saveFavData()
{
    
//    FavManager::getInstance()->saveFavData();
}

void AlumnAdapter::saveToFav(cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
//    saveFavCallback = callback;
//    FavManager::getInstance()->saveToFav("Fair Food",apCCImg);
    
}

void AlumnAdapter::saveToFav(const char *name, cocos2d::Image *apCCImg,const std::function<void()>& callback)
{
//    saveFavCallback = callback;
//    FavManager::getInstance()->saveToFav(name,apCCImg);
    
}

const char* AlumnAdapter::getFavImagePathByID(int tag)
{
    
//    FavManager::getInstance()->getFavImagePathByID(tag);
}

const char* AlumnAdapter::getFavDataPathByID(int tag)
{
//    FavManager::getInstance()->getFavDataPathByID(tag);
}

const char* AlumnAdapter::getFavNameByID(int tag)
{
//    FavManager::getInstance()->getFavImagePathByID(tag);
}

const char* AlumnAdapter::getFavIconNameByID(int tag)
{
//    FavManager::getInstance()->getFavIconPathByID(tag);
}

Image* AlumnAdapter::getFavIconByID(int tag)
{
//    FavManager::getInstance()->getFavIconByID(tag);
}

Image* AlumnAdapter::getFavImageByID(int tag)
{
//    FavManager::getInstance()->getFavImageByID(tag);
}

int AlumnAdapter::getFavCount()
{
//    FavManager::getInstance()->loadAllFavDataName("Fair Food");
//    FavManager::getInstance()->getFavCount();
}


