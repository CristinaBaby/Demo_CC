
#include "GameDataManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "IAPManager.h"
#include "StringHelp.h"

USING_NS_CC;

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

void GameDataManager::setFoodCount(int count)
{
    UserDefault::getInstance()->setIntegerForKey(kFoodCount, count);
    UserDefault::getInstance()->flush();
}

int GameDataManager::getFoodCount()
{
    return UserDefault::getInstance()->getIntegerForKey(kFoodCount, 1);
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

void GameDataManager::setMapLocked(bool locked)
{
    UserDefault::getInstance()->setBoolForKey("MAP", locked);
    UserDefault::getInstance()->flush();
}

bool GameDataManager::getMapLocked()
{
    return UserDefault::getInstance()->getBoolForKey("MAP", true);
}

void GameDataManager::setMapFinish(bool finish)
{
    UserDefault::getInstance()->setBoolForKey("MAP_FINISH", finish);
    UserDefault::getInstance()->flush();
}
bool GameDataManager::getMapFinish()
{
    return UserDefault::getInstance()->getBoolForKey("MAP_FINISH", false);
}
void GameDataManager::setIceShape(int count)
{
    UserDefault::getInstance()->setIntegerForKey(kIceShape, count);
    UserDefault::getInstance()->flush();
}

int GameDataManager::getIceShape()
{
    
    return UserDefault::getInstance()->getIntegerForKey(kIceShape, 0);
}

void GameDataManager::setStepCount(int count)
{
    if(count>getStepCount()){
        GameDataManager::getInstance()->m_bMapUnlock = false;
        UserDefault::getInstance()->setIntegerForKey("STEP", count);
        
        UserDefault::getInstance()->flush();
    }
}

int GameDataManager::getStepCount()
{
    return UserDefault::getInstance()->getIntegerForKey("STEP", 1);
}

void GameDataManager::setFlavorLocked(bool locked)
{
    UserDefault::getInstance()->setBoolForKey(kFlavorLock, locked);
    UserDefault::getInstance()->flush();
}
bool GameDataManager::getFlavorLocked()
{
    return UserDefault::getInstance()->getBoolForKey(kFlavorLock,true);
}