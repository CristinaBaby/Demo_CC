//
//  IAPManager.mm
//  Doctor
//
//  Created by tanshoumei on 13-8-12.
//
//

#import "IAPManager.h"
#include "cocos2d.h"

USING_NS_CC;


#define IAPCount 3

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static std::string IAPUrls[IAPCount] = {
    "com.animalhouseltd.petgrooming.unlockall",
    "com.animalhouseltd.petgrooming.allpets",
    "com.animalhouseltd.petgrooming.alldressup"
//    "com.happygirlmedia.android_summerprincessdress.billing_unlockall"
};


#else
static std::string IAPUrls[IAPCount] = {
    "com.animalhouseltd.petgrooming.unlockall",
    "com.animalhouseltd.petgrooming.allpets",
    "com.animalhouseltd.petgrooming.alldressup"
//    "com.shakeitmediainc.buffalowingsmaker.dips",
//    "com.shakeitmediainc.buffalowingsmaker.sides",
//    "com.shakeitmediainc.buffalowingsmaker.drinks",
//    "com.shakeitmediainc.buffalowingsmaker.plates"
};

#endif

IAPManager::IAPManager()
{
}

bool IAPManager::isAdRemoved()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return false;
#endif
    return getItemIsBought(kIAPUnlockAll);
}

bool IAPManager::isAllUnLocked()
{
    return getItemIsBought(0);
}

bool IAPManager::getItemIsBought(const int aInx)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    return true;
#endif
    if(aInx < 0 || aInx >= IAPCount){
        CCLOG("IAPManager::getItemIsBought invalid index:%d", aInx);
        return false;
    }
    else{
        string url = IAPUrls[aInx];
        return UserDefault::getInstance()->getBoolForKey(url.c_str());
    }
}

bool IAPManager::getItemIsBought(std::string aKey)
{
    return UserDefault::getInstance()->getBoolForKey(aKey.c_str());
}

std::string IAPManager::getItemKey(const int aInx)
{
    if(aInx < 0 || aInx >= IAPCount){
        CCLOG("IAPManager::getItemKey invalid index:%d", aInx);
        return "";
    }
    else{
        return IAPUrls[aInx];
    }
}

void IAPManager::setItemBoughtWithIndex(const int aInx)
{
    if(aInx < 0 || aInx >= IAPCount){
        CCLOG("IAPManager::setItemBoughtWithIndex invalid index:%d", aInx);
    }
    else{
        setItemBoughtWithID(IAPUrls[aInx]);
    }
}

void IAPManager::setItemBoughtWithID(std::string sId)
{
 
    CCLOG("IAPManager::setItemBoughtWithID:%s", sId.c_str());
    UserDefault::getInstance()->setBoolForKey(sId.c_str(), true);
    UserDefault::getInstance()->flush();
    if(sId.compare(IAPUrls[0]) == 0) //unlock all
    {
        //i=1意思是unlockall是放在数组的第一个位置的
        for (int i = 1; i < IAPCount; ++i) {
            setItemBoughtWithIndex(i);
        }
    }
    else if(!getItemIsBought(0)){
        //如果解锁的不是unlock all。则看是否其他（除了unlock all）都已解锁,如果其他都已解锁，自然应该解锁unlock all.
        bool unlockAll = true;
        for (int i = 1; i < IAPCount; ++i) {
            if(!getItemIsBought(i))
            {
                unlockAll = false;
                break;
            }
        }
        if(unlockAll)
            setItemBoughtWithIndex(0);
        
    }
    CCLOG("IAPManager::setItemBoughtWithID:%s", sId.c_str());
}





