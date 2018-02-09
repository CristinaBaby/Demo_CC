//
//  PensManager.cpp
//  TOYBM0701
//
//  Created by huangwen on 15/11/10.
//
//

#include "PensManager.h"
#include "UICpp.h"
#include "AppConfigs.h"
#include "UITools.h"
#include "TimerManager.hpp"
#include "AppConfigs.h"
#include "GameScene.h"
#include "LockManager.h"

#define COUNT      54
#define LOCK       30
#define MEMBER     5

PensManager* PensManager::msg = nullptr;

PensManager* PensManager::getInstance(){
    if (!msg) {
        msg = new PensManager();
    }
    return msg;
}

PensManager::PensManager(){
    setPensParchaseInfo("LocalPensPackInfo.plist");
}


void PensManager::setPensParchaseInfo(const std::string& path){
    auto root = FileUtils::getInstance()->getValueMapFromFile(path);
    _value = root["PensPack"].asValueVector();
    
}

std::string PensManager::getParchaseInfoFromIndex(int index){
    auto temp = _value.at(index).asValueMap();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return temp["iOS_PurchaseIdentifier"].asString();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return temp["Android_PurchaseIdentifier"].asString();
#else
#error "Write platform code here!"
#endif
}

std::vector<int>  PensManager::getPackInfoID(){
    vector<int> value;
    for(auto cava : _value)
    {
        value.push_back( cava.asValueMap()["packID"].asInt());
    }
    return value;
}

bool PensManager::pensLockSign(int group){
    log("-------cheak---%s",(RewardedAds_record_pens + std::to_string(group)).c_str());
    
    if(_user->getBoolForKey(string(RewardedAds_record_pens + std::to_string(group)).c_str(), false)){
        return true; //真为没锁
    }
    return false;
}

void PensManager::pensUnLock(int index, bool lock){
    _user->setBoolForKey(string(RewardedAds_record_pens + std::to_string(index)).c_str(), lock);
    _user->flush();
    log("----------pensUnLock--%s",(RewardedAds_record_pens + std::to_string(index)).c_str());
    
    if(lock)
        log("---------pens---------unlcok success!");
    else
        log("---------pens---------lcok success!");
}


bool PensManager::createBoxBackground(int index){
    
    if (index <= (COUNT - LOCK)) {
        log("Unlock PEN");
    }
    else
    {
        _sign++;
        if(_sign == MEMBER){
            _sign = 0;
        }
        
        if(_sign == 1)
        {
            int group = (index - (COUNT - LOCK)) / MEMBER;
            if (xLock->isLocked(group)) {
                //  枷锁
                return true;
            }
        }
    }
    return false;
}

bool PensManager::createBoxFrontground(int index){
    
    if (index <= (COUNT - LOCK)) {
        log("Unlock PEN");
    }
    else
    {
        _signFront++;
        if(_signFront == MEMBER){
            _signFront = 0;
            int group = (index - (COUNT - LOCK)) / MEMBER - 1;
            if (xLock->isLocked(group)) {
                //  枷锁
                return true;
            }
        }
    }
    return false;
}

bool PensManager::isHaveLock(int index){
    if (index <= (COUNT - LOCK)) {
        return false;
    }
    else{
        int group = (index - (COUNT - LOCK + 1)) / MEMBER;
        if (pensLockSign(group)) {
            return false;
        }
        return true;
    }
}

bool PensManager::getLockState(int index) {
    if (index <= (COUNT - LOCK)) {
        return false;
    }
    else{
        int group = index - (COUNT - LOCK + 1);
        if (pensLockSign(group)) {
            return false;
        }
        return true;
    }
}

int PensManager::getGroup(int index){
    
    if (index <= (COUNT - LOCK)) {
        CCASSERT(false, "Unlock PEN");
    }
    else
    {
        return ((index - (COUNT - LOCK + 1)) / MEMBER);
        
    }
    
    return 0;
}
