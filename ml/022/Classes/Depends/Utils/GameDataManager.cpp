
#include "GameDataManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "IAPManager.h"
#include "StringHelp.h"
//#include<stdlib.h>
//#include<cstdlib>

USING_NS_CC;

#define kFirstTime "FirstTime"
#define kCoint "Coint"
#define kBox "Box"
#define kPrice "Price"
#define kShapeLock "ShapeLock"
#define kFlavorLock "FlavorLock"
#define kIceShape "IceShape"
#define kFoodCount "FoodCount"
#define kStarData "StarData"

GameDataManager::GameDataManager()
{
    
}

GameDataManager::~GameDataManager()
{
    
}

void GameDataManager::setup()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int i = 0; i<4; i++) {
        std::stringstream ostr;
        ostr<<"Shop"<<i;
        if (GameDataAdapter::getBoolValue(ostr.str())) {
            IAPManager::getInstance()->setItemBoughtWithIndex(i);
        }
    }
#endif
}

void GameDataManager::setVideoUnlocked(std::string type,int index)
{
    std::string str = getVideoUnlockedString(type);
    std::stringstream ostr;
    ostr<<str<<index<<",";
    UserDefault::getInstance()->setStringForKey(type.c_str(),ostr.str().c_str());
    UserDefault::getInstance()->flush();
}
bool GameDataManager::getVideoUnlocked(std::string type,int index)
{
    std::string str = getVideoUnlockedString(type);
    if(str==""){
        return false;
    }
    std::vector<std::string> array = StringHelp::split(str, ",");
    bool unlocked = false;
    for (int i = 0; i<array.size(); i++) {
        std::string temp = array.at(i);
        int num = atoi(temp.c_str());
        if (num==index) {
            unlocked = true;
        }
    }
    return unlocked;
}

std::string GameDataManager::getVideoUnlockedString(std::string type)
{
    return UserDefault::getInstance()->getStringForKey(type.c_str(), "");
}
void GameDataManager::setShapeLocked(int index,bool locked)
{
    std::stringstream ostr;
    ostr<<kShapeLock<<index;
    UserDefault::getInstance()->setBoolForKey(ostr.str().c_str(), locked);
    UserDefault::getInstance()->flush();
}

bool GameDataManager::getShapeLocked(int index)
{
    std::stringstream ostr;
    ostr<<kShapeLock<<index;
    return UserDefault::getInstance()->getBoolForKey(ostr.str().c_str(), true);
}

void GameDataManager::setFirstTimePrice(bool isFirstTime)
{
    UserDefault::getInstance()->setBoolForKey(kFirstTime, isFirstTime);
    UserDefault::getInstance()->flush();
}
bool GameDataManager::getFirstTimePrice()
{
    return UserDefault::getInstance()->getBoolForKey(kFirstTime, true);
}
void GameDataManager::setBoxName(std::string data)
{
    UserDefault::getInstance()->setStringForKey(kBox, data);
    UserDefault::getInstance()->flush();
}
std::string GameDataManager::getBoxName()
{
    return UserDefault::getInstance()->getStringForKey(kBox, "");
}
void GameDataManager::setPriceData(int index,FoodData data)
{
    setPriceDataString(index, data.convertToString());
}

FoodData GameDataManager::getPriceData(int index)
{
    return FoodData::convertToFoodData(getPriceDataString(index));
}

void GameDataManager::setPriceDataString(int index,std::string data)
{
    std::stringstream ostr;
    ostr<<kPrice<<index;
    UserDefault::getInstance()->setStringForKey(ostr.str().c_str(), data);
    UserDefault::getInstance()->flush();
}
std::string GameDataManager::getPriceDataString(int index)
{
    std::stringstream ostr;
    ostr<<kPrice<<index;
    return UserDefault::getInstance()->getStringForKey(ostr.str().c_str(), "");
}


void GameDataManager::setCoint(float count)
{
    UserDefault::getInstance()->setFloatForKey(kCoint, count);
    UserDefault::getInstance()->flush();
}
float GameDataManager::getCoint()
{
    return UserDefault::getInstance()->getFloatForKey(kCoint, 0);
}