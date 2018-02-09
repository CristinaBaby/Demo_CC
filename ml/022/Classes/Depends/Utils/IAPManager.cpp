//
//  IAPManager.mm
//  Doctor
//
//  Created by tanshoumei on 13-8-12.
//
//

#include "IAPManager.h"
#include "cocos2d.h"
#include "ConfigManager.h"
#include "AdsManager.h"
#include "ConfigManager.h"
#include "GameDataManager.h"
#include "SSCIAPManager.h"

USING_NS_CC;


//#define IAPCount 4
//static std::string IAPUrls[IAPCount] = {
//    "com.bearhugmedia.celebritybabyworldcup.unlockall",
//    "com.bearhugmedia.celebritybabyworldcup.england",
//    "com.bearhugmedia.celebritybabyworldcup.argentine",
//    "com.bearhugmedia.celebritybabyworldcup.portugal",
//    "com.bearhugmedia.celebritybabyworldcup.france",
//    "com.bearhugmedia.celebritybabyworldcup.noads"
//};

IAPManager::IAPManager()
{
    IAPCount = ConfigManager::getInstance()->getIapDataVector().size();
    for (int i = 0; i<IAPCount; i++) {
        IAPConfigData data = ConfigManager::getInstance()->getIapData(i);
        IAPUnit* unit = IAPUnit::create("",data.iapId,data.iconName,data.unlockItem);
        
        m_pIapUnitVector.pushBack(unit);
    }
}

IAPManager::~IAPManager()
{
}

string IAPManager::getIapKeyString(unsigned int index)
{
    if(index >= IAPCount){
        CCLOG("IAPManager::getItemKey invalid index:%d", index);
        return "";
    }
    IAPUnit* unit = (IAPUnit*)m_pIapUnitVector.at(index);
    return unit->getKeyString();
}

string IAPManager::getIapId(unsigned int index)
{
    if(index >= IAPCount){
        CCLOG("IAPManager::getItemKey invalid index:%d", index);
        return "";
    }
    IAPUnit* unit = (IAPUnit*)m_pIapUnitVector.at(index);
    return unit->getIapID();
}

string IAPManager::getIapImageName(unsigned int index)
{
    if(index >= IAPCount){
        CCLOG("IAPManager::getItemKey invalid index:%d", index);
        return "";
    }
    IAPUnit* unit = (IAPUnit*)m_pIapUnitVector.at(index);
    return unit->getImageName();
}

std::vector<std::string> IAPManager::getIapTypeNames(unsigned int index)
{
    if(index >= IAPCount){
        CCLOG("IAPManager::getItemKey invalid index:%d", index);
        std::vector<std::string> temp;
        return temp;
    }
    IAPUnit* unit = (IAPUnit*)m_pIapUnitVector.at(index);
    return unit->getTypeNames();
}

bool IAPManager::getVideoUnLocked(std::string type,int index)
{
    return GameDataManager::getInstance()->getVideoUnlocked(type, index);
}

IAPUnit* IAPManager::getIapUnit(unsigned int index)
{
    return (IAPUnit*)m_pIapUnitVector.at(index);
}

void IAPManager::loadIapInfo(const char *fileaName)
{
    FileUtils *fu= FileUtils::getInstance();
    ValueVector vv;
    
    vv = fu->getValueVectorFromFile(fileaName);//dictionary,如果是vector,使用fu->getValueVectorFromFile(const std::string &filename)
    for_each(vv.begin(), vv.end(), [=](Value v){
        ValueMap map = v.asValueMap();
        IAPUnit* unit = IAPUnit::createWithDic(map);
        m_pIapUnitVector.pushBack(unit);
    });
}
bool IAPManager::isAdRemoved()
{
    //this app own ad for ever, so no removing.
    return UserDefault::getInstance()->getBoolForKey("removeAds");
}

bool IAPManager::isAllUnLocked()
{
    return getItemIsBought(0);
}

bool IAPManager::getItemIsBought(const int aInx)
{
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(aInx < 0 || aInx >= IAPCount){
        CCLOG("IAPManager::getItemIsBought invalid index:%d", aInx);
        return false;
    }
    else{
        string url = getIapKeyString(aInx);
        return UserDefault::getInstance()->getBoolForKey(url.c_str());
    }
#else
    return SSCIAPManager::getInstance()->isPackagePurchased(aInx+1);
#endif
}

bool IAPManager::getItemIsBought(std::string aKey)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return UserDefault::getInstance()->getBoolForKey(aKey.c_str());
#else
    return SSCIAPManager::getInstance()->isPackagePurchased(aKey);
#endif
}


void IAPManager::setItemBoughtWithIndex(const int aInx)
{
    if(aInx < 0 || aInx >= IAPCount){
        CCLOG("IAPManager::setItemBoughtWithIndex invalid index:%d", aInx);
    }
    else{
        setItemBoughtWithID(getIapKeyString(aInx));
        
    }
}

void IAPManager::setItemBoughtWithID(std::string sId)
{
    CCLOG("IAPManager::setItemBoughtWithID:%s", sId.c_str());
    UserDefault::getInstance()->setBoolForKey(sId.c_str(), true);
    UserDefault::getInstance()->flush();
    for (int i = 0; i<IAPCount; i++) {
        if(sId.compare(getIapKeyString(i)) == 0){
            CCLOG("unlock--%s",sId.c_str());
            ConfigManager::getInstance()->unlock(i);
            break;
        }
    }
//    unlock解锁  所有都解锁
    if(sId.compare(getIapKeyString(0)) == 0) //unlock all
    {
        //i=1意思是unlockall是放在数组的第一个位置的
        for (int i = 1; i < IAPCount ; i++) {
            setItemBoughtWithIndex(i);
        }
    }else {
    }
    
    if (getItemIsBought(0)) {
        if (!UserDefault::getInstance() -> getBoolForKey("removeAds")){
            
            AdsManager::getInstance()->removeAds(kTypeBannerAds);
        }
        UserDefault::getInstance()->setBoolForKey("removeAds", true);
    }
}




