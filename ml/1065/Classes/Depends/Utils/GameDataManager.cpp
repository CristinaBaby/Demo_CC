
#include "GameDataManager.h"
#include "cocos2d.h"
#include "GameDataAdapter.h"
#include "IAPManager.h"
#include "StringHelp.h"

USING_NS_CC;

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
}

int GameDataManager::getFoodCount()
{
    return UserDefault::getInstance()->getIntegerForKey(kFoodCount, 1);
}